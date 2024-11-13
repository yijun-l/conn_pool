#include "DBConnectionPool.h"

DBConnectionPool::DBConnectionPool(){
    
}

DBConnectionPool* DBConnectionPool::Instance(){
    static DBConnectionPool* const conn_pool = new DBConnectionPool();
    return conn_pool;
}

