#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

int main() {
    int *buffer = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *counter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    *counter = 0;
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid > 0) {
        for (int i = 1; i <= 5; i++) {
            while (*counter == 1)
                usleep(100000);
            *buffer = i;
            printf("Producer produced: %d\n", *buffer);
            *counter = 1;
            sleep(1);
        }
        wait(NULL);
    } else {
        for (int i = 1; i <= 5; i++) {
            while (*counter == 0)
                usleep(100000);
            printf("Consumer consumed: %d\n", *buffer);
            *counter = 0;
            sleep(1);
        }
    }

    munmap(buffer, sizeof(int));
    munmap(counter, sizeof(int));
    return 0;
}
