#pragma once
 
class DBConnectionPool{
public:
    static DBConnectionPool* Instance(); 

private:
    DBConnectionPool();
    ~DBConnectionPool();

    DBConnectionPool(const DBConnectionPool&) = delete;
    DBConnectionPool& operator=(const DBConnectionPool&) = delete;
};