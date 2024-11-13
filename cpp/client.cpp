#include <iostream>
#include "DBConnection.h"
#include "DBConnectionPool.h"

int main(){
    DBConnection* db_conn = new DBConnection();
    db_conn->query("select * from people where id = 1");
    delete db_conn;
    return 0;
}