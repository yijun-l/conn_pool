#ifndef _AV_CP_H
#define _AV_CP_H

/**
 * @brief Initializes the connection pool with worker threads and database connections.
 * @return int 0 on success, non-zero on failure.
 */
int cp_init();

/**
 * @brief Sends an SQL query to the connection pool for execution.
 */
void cp_query();

/**
 * @brief Prints the status of the connection pool, including worker thread info.
 */
void cp_stat();

/**
 * @brief Closes the connection pool and releases resources.
 * @return int 0 on success, non-zero on failure.
 */
int cp_finish();

#endif