#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int id, at, bt, rt, ct, tat, wt;
} Process;

int median(int arr[], int n) {
    for (int i=0; i<n-1; i++) {
        for (int j=i+1; j<n; j++) {
            if (arr[i] > arr[j]) {
                int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
            }
        }
    }
    if (n % 2 == 1) return arr[n/2];
    return (arr[n/2 - 1] + arr[n/2]) / 2;
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];
    for (int i=0; i<n; i++) {
        p[i].id = i+1;
        printf("P%d Arrival Time: ", i+1);
        scanf("%d", &p[i].at);
        printf("P%d Burst Time: ", i+1);
        scanf("%d", &p[i].bt);
        p[i].rt = p[i].bt;
    }

    int time = 0, done = 0, cycle = 1;
    printf("\nCycle | TQ | Processes\n");
    int tq;
    int rq[n], rq_size = 0;

    // Initial ready queue
    for (int i=0; i<n; i++) if (p[i].at == 0) rq[rq_size++] = i;
    if (rq_size > 0) {
        int arr[rq_size]; for (int i=0;i<rq_size;i++) arr[i] = p[rq[i]].bt;
        tq = median(arr, rq_size);
    } else tq = 1;

    printf("%d     | %d | [", cycle, tq);
    for (int i=0; i<rq_size; i++) printf("P%d ", p[rq[i]].id);
    printf("]\n");

    printf("\nGantt Chart: ");
    while (done < n) {
        int new_rq[n]; int new_size = 0;
        for (int i=0; i<rq_size; i++) {
            int idx = rq[i];
            if (p[idx].rt > tq) {
                printf("|%d-P%d-%d", time, p[idx].id, time+tq);
                time += tq;
                p[idx].rt -= tq;
                // check new arrivals during this slice
                for (int j=0;j<n;j++) {
                    if (p[j].at > time-tq && p[j].at <= time && p[j].rt>0) {
                        new_rq[new_size++] = j;
                    }
                }
                new_rq[new_size++] = idx; // requeue
            } else if (p[idx].rt > 0) {
                printf("|%d-P%d-%d", time, p[idx].id, time+p[idx].rt);
                time += p[idx].rt;
                p[idx].rt = 0;
                p[idx].ct = time;
                done++;
                for (int j=0;j<n;j++) {
                    if (p[j].at > time-p[idx].bt && p[j].at <= time && p[j].rt>0) {
                        new_rq[new_size++] = j;
                    }
                }
            }
        }
        printf("|");
        rq_size = new_size;
        for (int i=0;i<rq_size;i++) rq[i] = new_rq[i];

        if (rq_size > 0) {
            cycle++;
            int arr[rq_size]; for (int i=0;i<rq_size;i++) arr[i] = p[rq[i]].rt;
            tq = ceil((double)(arr[0]) / 2.0);
            for (int i=1;i<rq_size;i++) tq = ceil(((double)tq + arr[i]/2.0));
            tq = ceil((double)(arr[0]+arr[rq_size-1])/2.0); // approximate
            printf("\n%d     | %d | [", cycle, tq);
            for (int i=0; i<rq_size; i++) printf("P%d ", p[rq[i]].id);
            printf("]\n");
        }
    }

    printf("\n\nProcess AT  BT  CT  TAT WT\n");
    float avgTAT=0, avgWT=0;
    for (int i=0; i<n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        avgTAT += p[i].tat; avgWT += p[i].wt;
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, p[i].bt,
               p[i].ct, p[i].tat, p[i].wt);
    }
    printf("\nAvg TAT = %.2f, Avg WT = %.2f\n", avgTAT/n, avgWT/n);

    return 0;
}
