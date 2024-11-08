#include <stdio.h>
#include "libpq-fe.h"

int main() {
    const char *conninfo = "host=192.168.70.46 port=5432 dbname=postgres user=postgres password=123";
    // Makes a new connection to the database server.
    PGconn *conn = PQconnectdb(conninfo);

    // Returns the status of the connection.
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        // Closes the connection to the server. Also frees memory used by the PGconn object.
        PQfinish(conn);
        return 1;
    }

    printf("Connected to PostgreSQL server successfully!\n");

    // Read Action
    PGresult *res = PQexec(conn, "SELECT first_name, phone FROM people WHERE id > 9;");

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

     // Create Action
    res = PQexec(conn, "INSERT INTO people (first_name, age, phone) VALUES ('Andrew', 44, '555-012-6789');");
    PQclear(res);

    printf("\n");

    // Read Action
    res = PQexec(conn, "SELECT first_name, phone FROM people WHERE id > 9;");

    rows = PQntuples(res);
    cols = PQnfields(res);

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

    // Delete Action
    res = PQexec(conn, "DELETE FROM people WHERE first_name = 'Andrew';");
    PQclear(res);

    PQfinish(conn);
    return 0;
}

