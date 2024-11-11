#ifndef DB_CONNECTION_POOL_H
#define DB_CONNECTION_POOL_H

#include <pthread.h>
#include "task_queue.h"

// Connection pool structure
typedef struct db_connection_pool {
    task_queue* queue;            // Queue for SQL tasks
    pthread_mutex_t pool_mutex;   // Mutex for pool synchronization
    pthread_cond_t queue_not_full; // Condition for when queue is not full
    pthread_cond_t queue_not_empty; // Condition for when queue is not empty
    pthread_t* worker_threads;    // Array of worker threads
} db_connection_pool;

// Initializes the connection pool
db_connection_pool* db_connection_pool_init();

// Submits an SQL query to the connection pool
void db_connection_pool_query(db_connection_pool* pool, char* sql_query);

// Destroys the connection pool and frees resources
int db_connection_pool_destroy(db_connection_pool* pool);

#endif // DB_CONNECTION_POOL_H