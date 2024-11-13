#include "DBConnection.h"

DBConnection::DBConnection(){
    const char *conn_info = "host=192.168.70.46 port=5432 dbname=postgres user=postgres password=123";
    m_conn = PQconnectdb(conn_info);
    if (PQstatus(m_conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(m_conn));
        PQfinish(m_conn);
        m_conn = nullptr;
    }
}

DBConnection::~DBConnection()
{
    if(m_conn){
        PQfinish(m_conn);
    }
}

bool DBConnection::query(const char* sql)
{
    PGresult* result = PQexec(m_conn, sql);

    if (PQresultStatus(result) == PGRES_TUPLES_OK){
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
    }

    if (PQresultStatus(result) != PGRES_COMMAND_OK) {
        fprintf(stderr, "SQL execution failed: %s\n", PQerrorMessage(m_conn));
        PQclear(result);
        return false;
    }

    PQclear(result);
    return true;
}