#include <stdio.h>
#include "db_connection_pool.h"

int main() {
    db_connection_pool* pool = db_connection_pool_init();

    for (int i = 0; i < 10; i++){
        db_connection_pool_query(pool, "select * from people where id = 1");
        db_connection_pool_query(pool, "select * from people where id = 2");
        db_connection_pool_query(pool, "select * from people where id = 3");
    }

    printf("##################################\n");
    while(1);
 
    return 0;
}

