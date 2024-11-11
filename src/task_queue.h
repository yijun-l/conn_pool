#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

typedef struct {
    int capacity;  // Maximum capacity of the queue
    int size;      // Current number of tasks in the queue
    int front;     // Position for the next enqueue
    int rear;      // Position for the next dequeue
    char** tasks;  // Array to store tasks
} task_queue;

// Initializes the queue. Returns queue pointer on success, NULL on failure.
task_queue* task_queue_init(int queue_size);

// Checks if the queue is full. Returns 1 if full, 0 otherwise.
int task_queue_is_full(task_queue* queue);

// Checks if the queue is empty. Returns 1 if empty, 0 otherwise.
int task_queue_is_empty(task_queue* queue);

// Enqueues a task. Returns 0 on success, -1 on failure.
int task_queue_enqueue(task_queue* queue, const char* task);

// Dequeues a task. Returns pointer to task string on success, NULL on failure.
char* task_queue_dequeue(task_queue* queue);

// Destroys the queue and frees allocated memory.
void task_queue_destroy(task_queue* queue);

#endif // TASK_QUEUE_H