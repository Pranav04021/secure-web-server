#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> // For semaphores (optional, can use mutex+cond_vars)

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 1
#define NUM_CONSUMERS 1
#define ITEMS_TO_PRODUCE 10

int buffer[BUFFER_SIZE];
int in = 0; // Index for producer
int out = 0; // Index for consumer
int count = 0; // Current number of items in buffer

pthread_mutex_t mutex;
pthread_cond_t full_cond; // Condition variable for when buffer is full
pthread_cond_t empty_cond; // Condition variable for when buffer is empty

void *producer(void *arg) {
    int item;
    for (int i = 0; i < ITEMS_TO_PRODUCE; i++) {
        item = rand() % 100; // Produce a random item

        pthread_mutex_lock(&mutex); // Acquire lock

        while (count == BUFFER_SIZE) { // Buffer is full, wait
            printf("Producer: Buffer full, waiting...\n");
            pthread_cond_wait(&full_cond, &mutex);
        }

        buffer[in] = item;
        printf("Producer produced: %d at index %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;
        count++;

        pthread_cond_signal(&empty_cond); // Signal consumer that buffer is not empty
        pthread_mutex_unlock(&mutex); // Release lock
    }
    return NULL;
}

void *consumer(void *arg) {
    int item;
    for (int i = 0; i < ITEMS_TO_PRODUCE; i++) {
        pthread_mutex_lock(&mutex); // Acquire lock

        while (count == 0) { // Buffer is empty, wait
            printf("Consumer: Buffer empty, waiting...\n");
            pthread_cond_wait(&empty_cond, &mutex);
        }

        item = buffer[out];
        printf("Consumer consumed: %d from index %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&full_cond); // Signal producer that buffer is not full
        pthread_mutex_unlock(&mutex); // Release lock
    }
    return NULL;
}

int main() {
    pthread_t producer_threads[NUM_PRODUCERS];
    pthread_t consumer_threads[NUM_CONSUMERS];

    // Initialize mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&full_cond, NULL);
    pthread_cond_init(&empty_cond, NULL);

    // Create producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_create(&producer_threads[i], NULL, producer, NULL);
    }

    // Create consumer threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_create(&consumer_threads[i], NULL, consumer, NULL);
    }

    // Join producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producer_threads[i], NULL);
    }

    // Join consumer threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    // Destroy mutex and condition variables
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&full_cond);
    pthread_cond_destroy(&empty_cond);

    return 0;
}
