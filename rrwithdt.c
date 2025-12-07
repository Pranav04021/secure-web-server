#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct {
    char id[5];
    int at, bt, rt, ct, tat, wt;
    int finished;
} Process;

int main() {
    int n, i, time = 0, completed = 0;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];
    for (i = 0; i < n; i++) {
        printf("Enter ProcessID ArrivalTime BurstTime: ");
        scanf("%s %d %d", p[i].id, &p[i].at, &p[i].bt);
        p[i].rt = p[i].bt;
        p[i].finished = 0;
    }

    int tq, cycle = 1;
    printf("\n===== Gantt Chart =====\n");
    printf("%d", time);

    while (completed < n) {
        // Build ready queue
        int rq[n], rqSize = 0;
        for (i = 0; i < n; i++) {
            if (!p[i].finished && p[i].at <= time) rq[rqSize++] = i;
        }

        if (rqSize == 0) {
            time++;
            continue;
        }

        // Calculate TQ
        if (cycle == 1) {
            // First cycle → median of burst times
            int b[rqSize];
            for (i = 0; i < rqSize; i++) b[i] = p[rq[i]].bt;
            for (int a = 0; a < rqSize - 1; a++)
                for (int b2 = a + 1; b2 < rqSize; b2++)
                    if (b[a] > b[b2]) { int tmp = b[a]; b[a] = b[b2]; b[b2] = tmp; }
            if (rqSize % 2 == 1) tq = b[rqSize/2];
            else tq = (b[rqSize/2 - 1] + b[rqSize/2]) / 2;
        } else {
            // Next cycles → ceil(avg RT / 2)
            int sum = 0;
            for (i = 0; i < rqSize; i++) sum += p[rq[i]].rt;
            tq = (int)ceil((double)sum / rqSize / 2.0);
        }

        printf("\nCycle %d → TQ = %d, ReadyQ = [", cycle, tq);
        for (i = 0; i < rqSize; i++) printf("%s%s", p[rq[i]].id, (i==rqSize-1?"]":", "));

        // Execute RR for one cycle
        for (i = 0; i < rqSize; i++) {
            int idx = rq[i];
            if (p[idx].finished) continue;

            int run = (p[idx].rt > tq) ? tq : p[idx].rt;
            printf(" | %s | %d", p[idx].id, time + run);
            time += run;
            p[idx].rt -= run;

            if (p[idx].rt == 0) {
                p[idx].finished = 1;
                p[idx].ct = time;
                completed++;
            }
        }
        cycle++;
    }

    printf("\n========================\n");

    // Results
    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\n");
    float avgTAT=0, avgWT=0;
    for (i=0; i<n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        avgTAT += p[i].tat; avgWT += p[i].wt;
        printf("%s\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    printf("Average TAT = %.2f\n", avgTAT/n);
    printf("Average WT  = %.2f\n", avgWT/n);
    return 0;
}

