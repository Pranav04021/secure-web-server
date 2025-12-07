#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int id, at, bt, rt, ct, tat, wt;
} Process;

int main() {
    int n, c;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("P%d Arrival Time: ", i+1);
        scanf("%d", &p[i].at);
        printf("P%d Burst Time: ", i+1);
        scanf("%d", &p[i].bt);
        p[i].rt = p[i].bt;
    }
    printf("Enter context switch overhead: ");
    scanf("%d", &c);

    int completed = 0, t = 0, cs = 0, total_cs_time = 0;
    printf("\nGantt Chart: ");
    while (completed < n) {
        int idx = -1, min_rt = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= t && p[i].rt > 0 && p[i].rt < min_rt) {
                min_rt = p[i].rt;
                idx = i;
            }
        }

        if (idx == -1) { t++; continue; }

        printf("|%d-P%d", t, p[idx].id);
        p[idx].rt--;
        t++;

        // check preemption
        for (int j = 0; j < n; j++) {
            if (p[j].at == t && p[j].rt < p[idx].rt - c) {
                printf("-CS");
                t += c; total_cs_time += c; cs++;
                break;
            }
        }

        if (p[idx].rt == 0) {
            p[idx].ct = t;
            completed++;
            printf("|");
            if (completed < n) {
                printf("CS");
                t += c; total_cs_time += c; cs++;
            }
        }
    }

    printf("%d|\n", t);

    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\n");
    float avgTAT = 0, avgWT = 0;
    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        avgTAT += p[i].tat;
        avgWT += p[i].wt;
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    printf("\nAvg TAT = %.2f, Avg WT = %.2f", avgTAT/n, avgWT/n);
    printf("\nTotal Context Switches = %d, Total CS Time = %d", cs, total_cs_time);
    printf("\nCPU Efficiency = %.2f%%\n", 
        ((t - total_cs_time) * 100.0) / t);

    return 0;
}
