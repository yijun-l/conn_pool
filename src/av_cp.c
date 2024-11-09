#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "libpq-fe.h"

void* worker_routine(void* arg){
    const char *conninfo = "host=192.168.70.46 port=5432 dbname=postgres user=postgres password=123";
    // Makes a new connection to the database server.
    PGconn *conn = PQconnectdb(conninfo);

    // Returns the status of the connection.
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        // Closes the connection to the server. Also frees memory used by the PGconn object.
        PQfinish(conn);
        pthread_exit(NULL);
    }

    while (1){
        char buf[1024];
        PGresult *res;
        printf("AV_CP ==>");

        if (!fgets(buf, sizeof(buf), stdin) || !strcmp(buf, "q\n")){
            printf("Quit!\n");
            break;
        }

        res = PQexec(conn, buf);

        if (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK){
            printf("Incorrect Input, try again!\n");
            continue;
        }
        int rows = PQntuples(res);
        int cols = PQnfields(res);

        for (int i = 0; i < cols; i++) {
                printf("%-15s", PQfname(res, i));
        }
        printf("\n");
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                    printf("%-15s", PQgetvalue(res, i, j));
            }
            printf("\n");

        }
        PQclear(res);
     
    }
    PQfinish(conn);
    pthread_exit(NULL);
}

int cp_init(){
    // create process

    pthread_t worker_id;
    pthread_create(&worker_id, NULL, worker_routine, NULL);

    pthread_join(worker_id, NULL);
    return 0;
}