#include <stdio.h>

int main() {
    int frames, pages, page, i, j, pos = 0, faults = 0;
    printf("Enter number of frames: ");
    scanf("%d", &frames);
    printf("Enter number of pages: ");
    scanf("%d", &pages);

    int frame[frames];
    int ref[pages];

    printf("Enter page reference string:\n");
    for (i = 0; i < pages; i++)
        scanf("%d", &ref[i]);

    for (i = 0; i < frames; i++)
        frame[i] = -1;

    for (i = 0; i < pages; i++) {
        page = ref[i];
        int found = 0;

        for (j = 0; j < frames; j++)
            if (frame[j] == page)
                found = 1;

        if (!found) {
            frame[pos] = page;
            pos = (pos + 1) % frames;
            faults++;
        }
    }

    printf("Total Page Faults = %d\n", faults);
    return 0;
}
