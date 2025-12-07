#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define BUFFER_SIZE 5

int *buffer;
int *in, *out;
sem_t *empty, *full, *s;

void print_buffer() {
    printf("Buffer: [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (buffer[i] != 0)
            printf("%d", buffer[i]);
        else
            printf("_");
        if (i != BUFFER_SIZE - 1) printf(" ");
    }
    printf("]\n");
}

void producer(int n) {
    for (int i = 1; i <= n; i++) {
        sem_wait(empty);
        sem_wait(s);
        buffer[*in] = i;
        printf("Producer produced: %d\n", i);
        *in = (*in + 1) % BUFFER_SIZE;
        print_buffer();
        sem_post(s);
        sem_post(full);
        sleep(1);
    }
}

void consumer(int n) {
    for (int i = 1; i <= n; i++) {
        sem_wait(full);
        sem_wait(s);
        int item = buffer[*out];
        buffer[*out] = 0;
        printf("Consumer consumed: %d\n", item);
        *out = (*out + 1) % BUFFER_SIZE;
        print_buffer();
        sem_post(s);
        sem_post(empty);
        sleep(2);
    }
}

int main() {
    buffer = mmap(NULL, sizeof(int) * BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    in = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    out = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    empty = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    full = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    s = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    *in = *out = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) buffer[i] = 0;

    sem_init(empty, 1, BUFFER_SIZE);
    sem_init(full, 1, 0);
    sem_init(s, 1, 1);

    pid_t pid = fork();
    if (pid == 0)
        consumer(10);
    else {
        producer(10);
        wait(NULL);
    }

    sem_destroy(empty);
    sem_destroy(full);
    sem_destroy(s);
    munmap(buffer, sizeof(int) * BUFFER_SIZE);
    munmap(in, sizeof(int));
    munmap(out, sizeof(int));
    munmap(empty, sizeof(sem_t));
    munmap(full, sizeof(sem_t));
    munmap(s, sizeof(sem_t));
    return 0;
}
