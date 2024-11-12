#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "libpq-fe.h"
#include "db_connection_pool.h"

#define QUEUE_CAPACITY 5
#define WORKER_COUNT 5
// #define DEBUG
// #define PRINT

void* db_worker_thread(void* arg) {
    int thread_id = (int)pthread_self();

#ifdef DEBUG
    printf("[db_connection_pool] db_worker_thread(): Worker thread [%d] created.\n", thread_id);
#endif

    const char *conn_info = "host=192.168.70.46 port=5432 dbname=postgres user=postgres password=123";
    PGconn *conn = PQconnectdb(conn_info);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        pthread_exit(NULL);
    }

    // Register cleanup to close the connection on thread cancellation
    pthread_cleanup_push((void (*)(void *))PQfinish, conn);

    db_connection_pool* pool = (db_connection_pool*)arg;
    PGresult *result;

    while (1) {
        pthread_mutex_lock(&pool->pool_mutex);

        while (task_queue_is_empty(pool->queue)) {

#ifdef DEBUG
            printf("[db_connection_pool] db_worker_thread(): Worker thread [%d] waiting since task queue is empty\n", thread_id);
#endif
            pthread_cond_wait(&pool->queue_not_empty, &pool->pool_mutex);
        }

        char* sql_query = task_queue_dequeue(pool->queue);
        pthread_cond_signal(&pool->queue_not_full);
        pool->completed_tasks++;
        pthread_mutex_unlock(&pool->pool_mutex);

#ifdef DEBUG
        printf("[db_connection_pool] db_worker_thread(): Worker thread [%d] executing query: %s\n", thread_id, sql_query);
#endif

        result = PQexec(conn, sql_query);
        free(sql_query);  // Free memory after execution

        if (PQresultStatus(result) != PGRES_COMMAND_OK && PQresultStatus(result) != PGRES_TUPLES_OK) {
            fprintf(stderr, "Query execution failed: %s\n", PQerrorMessage(conn));
            PQclear(result);
            continue;
        }
#ifdef PRINT
        int rows = PQntuples(result);
        int columns = PQnfields(result);

        for (int i = 0; i < columns; i++) {
            printf("%-15s", PQfname(result, i));
        }
        printf("\n");

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                printf("%-15s", PQgetvalue(result, i, j));
            }
            printf("\n");
        }
#endif

        PQclear(result);
#ifdef DEBUG
        printf("[db_connection_pool] db_worker_thread(): Worker thread [%d] finished query execution\n", thread_id);
#endif
    }

    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}

db_connection_pool* db_connection_pool_init() {
    db_connection_pool* pool = (db_connection_pool*)malloc(sizeof(db_connection_pool));
    if (!pool) return NULL;

    pool->queue = task_queue_init(QUEUE_CAPACITY);
    if (!pool->queue) {
        free(pool);
        return NULL;
    }
    pool->completed_tasks = 0;
    pthread_mutex_init(&pool->pool_mutex, NULL);
    pthread_cond_init(&pool->queue_not_full, NULL);
    pthread_cond_init(&pool->queue_not_empty, NULL);

    pool->worker_threads = (pthread_t*)calloc(WORKER_COUNT, sizeof(pthread_t));
    if (!pool->worker_threads) {
        task_queue_destroy(pool->queue);
        free(pool);
        return NULL;
    }

    for (int i = 0; i < WORKER_COUNT; i++) {
        pthread_create(&pool->worker_threads[i], NULL, db_worker_thread, (void*)pool);
    }

    return pool;
}

void db_connection_pool_query(db_connection_pool* pool, char* sql_query) {
    pthread_mutex_lock(&pool->pool_mutex);

    while (task_queue_is_full(pool->queue)) {
#ifdef DEBUG
        printf("[db_connection_pool] db_connection_pool_query(): Query request blocked since task queue is full\n");
#endif
        pthread_cond_wait(&pool->queue_not_full, &pool->pool_mutex);
    }

    task_queue_enqueue(pool->queue, sql_query);
    pthread_cond_signal(&pool->queue_not_empty);
    pthread_mutex_unlock(&pool->pool_mutex);
}

void db_connection_status(db_connection_pool* pool){
    time_t current_time;
    time(&current_time);

    struct tm *local_time = localtime(&current_time);

    printf("---- Connection Pool Status ----\nCurrent time: %02d:%02d:%02d\nActive workers: %d\nTask Queue size: %d\nCompleted tasks: %d\n", local_time->tm_hour, local_time->tm_min, local_time->tm_sec, WORKER_COUNT, QUEUE_CAPACITY, pool->completed_tasks);
}


int db_connection_pool_destroy(db_connection_pool* pool) {
    if (!pool) return -1;

    for (int i = 0; i < WORKER_COUNT; i++) {
        pthread_cancel(pool->worker_threads[i]);
        pthread_join(pool->worker_threads[i], NULL);
    }

    pthread_mutex_destroy(&pool->pool_mutex);
    pthread_cond_destroy(&pool->queue_not_full);
    pthread_cond_destroy(&pool->queue_not_empty);

    task_queue_destroy(pool->queue);
    free(pool->worker_threads);
    free(pool);

    return 0;
}
