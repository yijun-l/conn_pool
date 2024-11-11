#include "task_queue.h"
#include <stdlib.h>
#include <string.h>

task_queue* task_queue_init(int queue_size) {
    task_queue* queue = (task_queue*)malloc(sizeof(task_queue));
    if (queue == NULL) {
        return NULL; // Allocation failed, return NULL
    }

    queue->capacity = queue_size;
    queue->size = 0;
    queue->front = 0;
    queue->rear = 0;
    queue->tasks = (char**)calloc(queue_size, sizeof(char*));
    if (queue->tasks == NULL) {
        free(queue); // Free allocated memory if calloc fails
        return NULL;
    }
    
    return queue;
}

int task_queue_is_full(task_queue* queue) {
    return queue->size == queue->capacity;
}

int task_queue_is_empty(task_queue* queue) {
    return queue->size == 0;
}

int task_queue_enqueue(task_queue* queue, const char* task) {
    if (task_queue_is_full(queue)) {
        return -1; // Queue is full, cannot enqueue
    }

    queue->tasks[queue->front] = strdup(task);
    if (queue->tasks[queue->front] == NULL) {
        return -1; // Memory allocation failed
    }

    queue->front = (queue->front + 1) % queue->capacity;
    queue->size++;
    return 0;
}

char* task_queue_dequeue(task_queue* queue) {
    if (task_queue_is_empty(queue)) {
        return NULL; // Queue is empty, cannot dequeue
    }

    char* task = queue->tasks[queue->rear];
    queue->tasks[queue->rear] = NULL;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->size--;
    return task;
}

void task_queue_destroy(task_queue* queue) {
    if (queue == NULL) {
        return;
    }

    for (int i = 0; i < queue->capacity; i++) {
        if (queue->tasks[i] != NULL) {
            free(queue->tasks[i]);
        }
    }
    free(queue->tasks);
    free(queue);
}
