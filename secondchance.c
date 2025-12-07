#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int page_num;
    int ref_bit;
} PageFrame;

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
    PageFrame *frame = (PageFrame *)malloc(capacity * sizeof(PageFrame));

    if (incomingStream == NULL || frame == NULL) {
        printf("Memory allocation failed.\n");
        free(incomingStream); free(frame);
        return 1;
    }

    printf("Enter the reference string (space-separated integers):\n");
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &incomingStream[i]) != 1) {
            printf("Invalid input for reference string.\n");
            free(incomingStream); free(frame);
            return 1;
        }
    }

    int hand = 0;
    int hits = 0;
    int faults = 0;

    for (int i = 0; i < capacity; i++) {
        frame[i].page_num = -1;
        frame[i].ref_bit = 0;
    }

    for (int i = 0; i < n; i++) {
        int page = incomingStream[i];
        bool found = false;
        
        for (int j = 0; j < capacity; j++) {
            if (frame[j].page_num == page) {
                found = true;
                hits++;
                frame[j].ref_bit = 1;
                break;
            }
        }

        if (!found) {
            faults++;
            while (true) {
                if (frame[hand].page_num == -1) {
                    frame[hand].page_num = page;
                    frame[hand].ref_bit = 1;
                    hand = (hand + 1) % capacity;
                    break;
                }
                
                if (frame[hand].ref_bit == 0) {
                    frame[hand].page_num = page;
                    frame[hand].ref_bit = 1;
                    hand = (hand + 1) % capacity;
                    break;
                } else {
                    frame[hand].ref_bit = 0;
                    hand = (hand + 1) % capacity;
                }
            }
        }
    }

    printf("\n--- Second Chance (Clock) Results ---\n");
    printf("Capacity: %d\n", capacity);
    printf("Total References: %d\n", n);
    printf("Page Faults: %d\n", faults);
    printf("Page Hits: %d\n", hits);
    
    free(incomingStream);
    free(frame);
    return 0;
}
