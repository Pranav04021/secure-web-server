#include <stdio.h>
#include <stdbool.h>

int main() {
    int capacity = 3;
    int incomingStream[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0};
    int n = sizeof(incomingStream) / sizeof(incomingStream[0]);
    int frame[capacity];
    int front = 0;
    int hits = 0;
    int faults = 0;

    for (int i = 0; i < capacity; i++) {
        frame[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        int page = incomingStream[i];
        bool found = false;
        
        for (int j = 0; j < capacity; j++) {
            if (frame[j] == page) {
                found = true;
                hits++;
                break;
            }
        }

        if (!found) {
            faults++;
            frame[front] = page;
            front = (front + 1) % capacity;
        }
    }

    printf("FIFO Page Replacement Example\n");
    printf("Capacity: %d\n", capacity);
    printf("Reference String: 7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0\n");
    printf("Page Faults: %d\n", faults);
    printf("Page Hits: %d\n", hits);
    return 0;
}
