# Multi-threading

Multi-threading allows a program to perform multiple operations concurrently by dividing tasks into smaller threads, each capable of executing independently while sharing the same memory space. This approach not only maximizes CPU usage but also improves application responsiveness, especially in scenarios involving I/O operations, data processing, or user interaction. In a multithreaded environment, threads can run in parallel on multi-core processors, significantly enhancing performance and enabling complex, resource-intensive applications to manage multiple tasks simultaneously.

## pthread Library

In Linux, the POSIX thread (or pthread) library provides a standardized set of APIs specifically designed for this purpose, offering developers tools to create, manage, and synchronize threads effectively.

Below are definitions of several essential pthread functions:

### pthread_create

Creates a new thread.

```c
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                     void *(*start_routine) (void *), void *arg);

```

- `thread`: Pointer to the thread identifier.
- `attr`: Thread attributes (use NULL for default).
- `start_routine`: Pointer to the function the thread will execute.
- `arg`: Argument passed to the `start_routine` function.


### pthread_exit

Terminates the calling thread and optionally returns a value to pthread_join.

```c
void pthread_exit(void *retval);
```

- `retval`: Pointer to the value returned by the thread.


### pthread_join

Waits for a specific thread to terminate and retrieves its return value.

```c
int pthread_join(pthread_t thread, void **retval);
```

- `thread`: The thread to join.
- `retval`: Address of a pointer to hold the thread's return value.


### pthread_detach

Marks a thread as detached, allowing it to release its resources upon termination automatically.

```c
int pthread_detach(pthread_t thread);
```
- `thread`: The thread to detach.


### pthread_cancel

Sends a cancellation request to a specific thread.

```c
int pthread_cancel(pthread_t thread);
```

- `thread`: The thread to cancel.


