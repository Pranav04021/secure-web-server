#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

int findLFU(int frame[], int frequency[], int capacity) {
    int min_val = INT_MAX;
    int min_idx = -1;
    for (int i = 0; i < capacity; i++) {
        // Tie-breaking: if frequency is same, choose the one that entered first (lower index)
        if (frame[i] != -1 && frequency[i] < min_val) {
            min_val = frequency[i];
            min_idx = i;
        }
    }
    return min_idx;
}

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
    int *frame = (int *)malloc(capacity * sizeof(int));
    int *frequency = (int *)malloc(capacity * sizeof(int));

    if (incomingStream == NULL || frame == NULL || frequency == NULL) {
        printf("Memory allocation failed.\n");
        free(incomingStream); free(frame); free(frequency);
        return 1;
    }

    printf("Enter the reference string (space-separated integers):\n");
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &incomingStream[i]) != 1) {
            printf("Invalid input for reference string.\n");
            free(incomingStream); free(frame); free(frequency);
            return 1;
        }
    }

    int hits = 0;
    int faults = 0;

    for (int i = 0; i < capacity; i++) {
        frame[i] = -1;
        frequency[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int page = incomingStream[i];
        bool found = false;
        
        for (int j = 0; j < capacity; j++) {
            if (frame[j] == page) {
                found = true;
                hits++;
                frequency[j]++;
                break;
            }
        }

        if (!found) {
            faults++;
            int empty_idx = -1;
            for(int j = 0; j < capacity; j++) {
                if (frame[j] == -1) {
                    empty_idx = j;
                    break;
                }
            }

            if (empty_idx != -1) {
                frame[empty_idx] = page;
                frequency[empty_idx] = 1;
            } else {
                int lfu_idx = findLFU(frame, frequency, capacity);
                frame[lfu_idx] = page;
                frequency[lfu_idx] = 1;
            }
        }
    }

    printf("\n--- LFU Results ---\n");
    printf("Capacity: %d\n", capacity);
    printf("Total References: %d\n", n);
    printf("Page Faults: %d\n", faults);
    printf("Page Hits: %d\n", hits);
    
    free(incomingStream);
    free(frame);
    free(frequency);
    return 0;
}
