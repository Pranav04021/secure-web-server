#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define BUFFER_SIZE 5

int main() {
    int *buffer = mmap(NULL, sizeof(int) * BUFFER_SIZE, PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *counter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *in = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                   MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *out = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    *counter = 0;
    *in = 0;
    *out = 0;

    pid_t pid = fork();

    if (pid == 0) { 
        // Consumer Process
        for (int i = 0; i < 10; i++) {
            while (*counter == 0) 
                ; // Wait if buffer empty

            int item = buffer[*out];
            printf("Consumer consumed: %d\n", item);
            *out = (*out + 1) % BUFFER_SIZE;
            (*counter)--;
            sleep(2);
        }
    } 
    else { 
        // Producer Process
        for (int i = 1; i <= 10; i++) {
            while (*counter == BUFFER_SIZE) 
                ; // Wait if buffer full

            buffer[*in] = i;
            printf("Producer produced: %d\n", i);
            *in = (*in + 1) % BUFFER_SIZE;
            (*counter)++;
            sleep(1);
        }
        wait(NULL);
    }

    munmap(buffer, sizeof(int) * BUFFER_SIZE);
    munmap(counter, sizeof(int));
    munmap(in, sizeof(int));
    munmap(out, sizeof(int));

    return 0;
}
