#include <stdio.h>
#include "db_connection_pool.h"

int main() {
    db_connection_pool* pool = db_connection_pool_init();

    db_connection_status(pool);
    for (int i = 0; i < 50100; i++){
        db_connection_pool_query(pool, "select * from people where id = 1");
    }
    printf("##################################\n");

    db_connection_status(pool);
 
    return 0;
}

