#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main() {
    int capacity, n;

    printf("Enter the frame capacity: ");
    if (scanf("%d", &capacity) != 1 || capacity <= 0) {
        printf("Invalid capacity input.\n");
        return 1;
    }

    printf("Enter the length of the reference string: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid length input.\n");
        return 1;
    }

    int *incomingStream = (int *)malloc(n * sizeof(int));
    if (incomingStream == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter the reference string (space-separated integers):\n");
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &incomingStream[i]) != 1) {
            printf("Invalid input for reference string.\n");
            free(incomingStream);
            return 1;
        }
    }

    int *frame = (int *)malloc(capacity * sizeof(int));
    if (frame == NULL) {
        printf("Memory allocation failed.\n");
        free(incomingStream);
        return 1;
    }

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

    printf("\n--- FIFO Results ---\n");
    printf("Capacity: %d\n", capacity);
    printf("Total References: %d\n", n);
    printf("Page Faults: %d\n", faults);
    printf("Page Hits: %d\n", hits);

    free(incomingStream);
    free(frame);
    return 0;
}
