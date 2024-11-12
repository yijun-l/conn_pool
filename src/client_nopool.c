#include <stdio.h>
#include <time.h>
#include "libpq-fe.h"

void print_count(time_t* current_time, int count){
    time(current_time);
    struct tm *local_time = localtime(current_time);
    printf("---- Status without Connection Pool ----\nCurrent time: %02d:%02d:%02d\nCompleted tasks: %d\n", local_time->tm_hour, local_time->tm_min, local_time->tm_sec, count);
}

int main(){
    const char *conn_info = "host=192.168.70.46 port=5432 dbname=postgres user=postgres password=123";
    PGconn *conn = PQconnectdb(conn_info);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return 1;
    }

    PGresult *result;
    int count = 0;
    time_t current_time;

    print_count(&current_time, count);

    for (int i = 0; i < 5000; i++){
        result = PQexec(conn, "select * from people where id = 1");
        PQclear(result);
        count++;
    }
    printf("##################################\n");
    print_count(&current_time, count);

    PQfinish(conn);
    return 0;
}