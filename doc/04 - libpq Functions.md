# libpq Library

As introduced, `libpq` is the core library used to communicate with a PostgreSQL server, and we’ve already used several libpq functions in earlier code.

Below is an overview of some essential `libpq` functions. For a complete list and more details, refer to the official PostgreSQL documentation: https://www.postgresql.org/files/documentation/pdf/13/postgresql-13-A4.pdf

## Database Connection Control Functions

### PQconnectdb

Establishes a new connection to the database server.

```c
PGconn *PQconnectdb(const char *conninfo);
```

This function opens a database connection using connection parameters provided in the `conninfo` string.


### PQfinish

Closes the connection to the server and frees the memory allocated for the `PGconn` object.

```c
void PQfinish(PGconn *conn);
```

### PQstatus

Returns the current status of the connection.

```c
ConnStatusType PQstatus(const PGconn *conn);
```

The status can take several values, but only two are typically seen outside asynchronous connection handling:

- `CONNECTION_OK`: The connection to the database is successful.
- `CONNECTION_BAD`: The connection attempt failed.


### PQerrorMessage

Retrieves the most recent error message for operations on the connection.

```c
char *PQerrorMessage(const PGconn *conn);
```

Most `libpq` functions will set a message for `PQerrorMessage` if they encounter an error.

<br>

## Command Execution Functions

### PQexec

Sends a command to the server and waits for the result.

```c
PGresult *PQexec(PGconn *conn, const char *command);
```

Returns a `PGresult` pointer on success, or `NULL` on serious errors like out-of-memory conditions or failure to send the command to the server.

###  PQresultStatus

Retrieves the status of the command result.

```c
ExecStatusType PQresultStatus(const PGresult *res);
```

Key return values:

- `PGRES_COMMAND_OK`: Indicates successful execution of a command that does not return data.
- `PGRES_TUPLES_OK`: Indicates successful execution of a command that returns data, such as `SELECT` or `SHOW`.

### PQclear

Releases the memory associated with a `PGresult`. Every command result should be freed with `PQclear` when it is no longer needed.

```c
void PQclear(PGresult *res);
```

The `PGresult` object remains valid until `PQclear` is called, even if a new command is issued or the connection is closed. Failing to call `PQclear` will cause memory leaks.

<br>

## Retrieving Query Result Functions

### PQntuples

Returns the number of rows (tuples) in the query result.

```c
int PQntuples(const PGresult *res);
```

### PQnfields

Returns the number of columns (fields) in each row of the query result.

```c
int PQnfields(const PGresult *res);
```


### PQfname

Gets the column name for a given column number. Column numbers start at 0.


```c
char *PQfname(const PGresult *res, int column_number);
```

If the column number is out of range, the function returns `NULL`.

### PQgetvalue

Retrieves the value of a specific field in a particular row of the `PGresult`. Row and column numbers start at 0.

```c
char *PQgetvalue(const PGresult *res, int row_number, int column_number);
```

<br>

# Connection Pool API design

Using multiple libpq functions directly can be complex for client applications. In our project, the client application only needs to send SQL queries to the connection pool.

To simplify this process, we encapsulate the core functions as follows:

- `cp_init()`: Initializes the connection pool by creating worker threads and establishing initial database connections.

- `cp_query()`: Sends an SQL query to the database by dispatching it to a worker thread that manages query execution.

- `cp_stat()`: Prints the status of the connection pool, summarizing the state of each worker thread and reporting the number of completed tasks.

- `cp_finish()`:  Shuts down the connection pool by closing all active database connections and releasing worker threads.