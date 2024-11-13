#pragma once
#include "libpq-fe.h"

class DBConnection{
public:
    DBConnection();
    ~DBConnection();
    bool query(const char* sql);

private:
    PGconn* m_conn = nullptr;
};

