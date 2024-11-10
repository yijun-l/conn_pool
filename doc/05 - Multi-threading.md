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

# Thread Synchronization

In a multi-threaded environment, where threads share the same memory space, ensuring safe access to shared resources is crucial to prevent data races and maintain program correctness. Thread synchronization mechanisms in the pthread library are essential for coordinating threads and managing shared resources effectively. Two primary synchronization tools provided by pthread are **mutexes** (mutual exclusion locks) and **condition variables**.

## Mutexes

A mutex (mutual exclusion lock) is a synchronization primitive that allows only one thread to access a shared resource at a time. This prevents race conditions, where multiple threads attempt to read or modify shared data concurrently, leading to unpredictable outcomes. 

A mutex is typically locked by a thread before accessing a critical section and unlocked afterward, ensuring exclusive access to shared resources.

### pthread_mutex_init

Initializes a mutex with specified attributes, preparing it for use.

```c
// Dynamic initialization
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);

// Static initialization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
```
- `mutex`: Pointer to the mutex to initialize.
- `attr`: Pointer to the mutex attributes (use NULL for default attributes).


### pthread_mutex_destroy

Destroys a mutex, releasing any resources it holds. Only a mutex that is not currently locked by any thread should be destroyed.

```c
int pthread_mutex_destroy(pthread_mutex_t *mutex);
```

- `mutex`: Pointer to the mutex to destroy.

### pthread_mutex_lock

Locks a mutex, blocking the calling thread if the mutex is already locked by another thread. This ensures that only one thread accesses the critical section at a time.


```c
int pthread_mutex_lock(pthread_mutex_t *mutex);
```
- `mutex`: Pointer to the mutex to lock.


### pthread_mutex_trylock

Attempts to lock a mutex without blocking. If the mutex is already locked by another thread, this function returns immediately with an error code instead of blocking.


```c
int pthread_mutex_trylock(pthread_mutex_t *mutex);
```
- `mutex`: Pointer to the mutex to attempt locking.


### pthread_mutex_unlock
Unlocks a previously locked mutex, allowing other threads to acquire it.

```c
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

- `mutex`: Pointer to the mutex to unlock.


## Condition Variables

Condition variables enable threads to communicate with each other by signaling events. They are particularly useful when a thread needs to wait until a certain condition is met before proceeding. Condition variables work alongside mutexes to allow threads to wait for specific conditions and notify other threads when those conditions are satisfied, providing a way to manage complex dependencies between threads.

### pthread_cond_init

Initializes a condition variable with specified attributes, preparing it for use.

```c
// Dynamic initialization
int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);

// Static initialization
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
```

- `cond`: Pointer to the condition variable to initialize.
- `attr`: Pointer to the condition variable attributes (use NULL for default attributes).

### pthread_cond_destroy

Destroys a condition variable, releasing any resources it holds.

```c
int pthread_cond_destroy(pthread_cond_t *cond);
```

- `cond`: Pointer to the condition variable to destroy.

### pthread_cond_wait

Blocks the calling thread until the specified condition variable is signaled. This function must be called while holding a mutex.

```c

int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);

```

- `cond`: Pointer to the condition variable.
- `mutex`: Pointer to the mutex, which will be released during the wait and re-acquired when the thread is awakened.


### pthread_cond_signal

Wakes up one thread that is waiting on the specified condition variable.

```c
int pthread_cond_signal(pthread_cond_t *cond);
```
- `cond`: Pointer to the condition variable.

### pthread_cond_broadcast

Wakes up all threads that are waiting on the specified condition variable.

```c
int pthread_cond_broadcast(pthread_cond_t *cond);
```

- `cond`: Pointer to the condition variable.