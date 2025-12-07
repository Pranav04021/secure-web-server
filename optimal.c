#include <stdio.h>

int findOptimal(int ref[], int frame[], int n, int index, int frames) {
    int pos = -1, farthest = index, i, j;
    for (i = 0; i < frames; i++) {
        int found = 0;
        for (j = index; j < n; j++) {
            if (frame[i] == ref[j]) {
                found = 1;
                if (j > farthest) {
                    farthest = j;
                    pos = i;
                }
                break;
            }
        }
        if (!found)
            return i;
    }
    return (pos == -1) ? 0 : pos;
}

int main() {
    int frames, pages, i, j, faults = 0;
    printf("Enter number of frames: ");
    scanf("%d", &frames);
    printf("Enter number of pages: ");
    scanf("%d", &pages);

    int ref[pages], frame[frames];

    printf("Enter reference string:\n");
    for (i = 0; i < pages; i++)
        scanf("%d", &ref[i]);

    for (i = 0; i < frames; i++)
        frame[i] = -1;

    for (i = 0; i < pages; i++) {
        int found = 0;

        for (j = 0; j < frames; j++)
            if (frame[j] == ref[i])
                found = 1;

        if (!found) {
            int pos = findOptimal(ref, frame, pages, i + 1, frames);
            frame[pos] = ref[i];
            faults++;
        }
    }

    printf("Total Page Faults = %d\n", faults);
    return 0;
}
