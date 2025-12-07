#include <stdio.h>

int findOptimal(int ref[], int frame[], int n, int index, int frames) {
    int pos = -1, farthest = -1;

    for (int i = 0; i < frames; i++) {
        int j;
        for (j = index; j < n; j++) {
            if (frame[i] == ref[j]) {
                if (j > farthest) {
                    farthest = j;
                    pos = i;
                }
                break;
            }
        }
        if (j == n) {
            return i; // never used again → best to replace
        }
    }

    return (pos == -1) ? 0 : pos;
}

int main() {
    int frames, pages;

    printf("Enter number of frames: ");
    scanf("%d", &frames);

    printf("Enter number of pages: ");
    scanf("%d", &pages);

    int ref[pages], frame[frames];

    printf("Enter reference string:\n");
    for (int i = 0; i < pages; i++)
        scanf("%d", &ref[i]);

    for (int i = 0; i < frames; i++)
        frame[i] = -1;

    int faults = 0;

    for (int i = 0; i < pages; i++) {

        int found = 0;
        for (int j = 0; j < frames; j++)
            if (frame[j] == ref[i])
                found = 1;

        if (!found) {
            int empty = -1;
            for (int j = 0; j < frames; j++)
                if (frame[j] == -1) {
                    empty = j;
                    break;
                }

            if (empty != -1)
                frame[empty] = ref[i];
            else {
                int pos = findOptimal(ref, frame, pages, i + 1, frames);
                frame[pos] = ref[i];
            }

            faults++;
        }
    }

    printf("Total Page Faults = %d\n", faults);
    return 0;
}
