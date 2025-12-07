#include <stdio.h>

int findLRU(int time[], int n) {
    int i, min = time[0], pos = 0;
    for (i = 1; i < n; i++)
        if (time[i] < min) {
            min = time[i];
            pos = i;
        }
    return pos;
}

int main() {
    int frames, pages, i, j, faults = 0, counter = 0;
    printf("Enter number of frames: ");
    scanf("%d", &frames);
    printf("Enter number of pages: ");
    scanf("%d", &pages);

    int ref[pages], frame[frames], time[frames];

    printf("Enter page reference string:\n");
    for (i = 0; i < pages; i++)
        scanf("%d", &ref[i]);

    for (i = 0; i < frames; i++)
        frame[i] = -1;

    for (i = 0; i < pages; i++) {
        int page = ref[i];
        int found = 0;

        for (j = 0; j < frames; j++)
            if (frame[j] == page) {
                found = 1;
                time[j] = counter++;
            }

        if (!found) {
            int pos = -1;
            for (j = 0; j < frames; j++)
                if (frame[j] == -1) {
                    pos = j;
                    break;
                }

            if (pos == -1)
                pos = findLRU(time, frames);

            frame[pos] = page;
            time[pos] = counter++;
            faults++;
        }
    }

    printf("Total Page Faults = %d\n", faults);
    return 0;
}
