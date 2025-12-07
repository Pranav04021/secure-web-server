#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id[5];
    int at, bt, rt, ct, tat, wt;
    int finished;
} Process;

int main() {
    int n, i, time = 0, completed = 0, c;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    Process p[n];

    for (i = 0; i < n; i++) {
        printf("Enter ProcessID ArrivalTime BurstTime: ");
        scanf("%s %d %d", p[i].id, &p[i].at, &p[i].bt);
        p[i].rt = p[i].bt;
        p[i].finished = 0;
    }

    printf("Enter context switch overhead C: ");
    scanf("%d", &c);

    int last = -1, csCount = 0, csTime = 0;

    printf("\n===== Gantt Chart =====\n");
    printf("%d", time);

    while (completed < n) {
        int idx = -1, minRT = 1e9;

        // Choose process with shortest remaining time (considering overhead)
        for (i = 0; i < n; i++) {
            if (!p[i].finished && p[i].at <= time) {
                if (idx == -1 || p[i].rt < minRT) {
                    if (last != -1) {
                        if (p[last].rt > p[i].rt + c) {
                            idx = i;
                            minRT = p[i].rt;
                        }
                    } else {
                        idx = i;
                        minRT = p[i].rt;
                    }
                }
            }
        }

        if (idx == -1) {  // CPU idle
            time++;
            continue;
        }

        if (last != -1 && last != idx) {  // Context switch
            printf(" | CS | %d", time);
            time += c;
            csTime += c;
            csCount++;
        }

        // Run process for 1 unit
        printf(" | %s | %d", p[idx].id, time + 1);
        p[idx].rt--;
        time++;

        if (p[idx].rt == 0) {
            p[idx].finished = 1;
            p[idx].ct = time;
            completed++;
        }
        last = idx;
    }

    printf("\n========================\n");

    // Results table
    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\n");
    float avgTAT = 0, avgWT = 0;
    for (i = 0; i < n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        avgTAT += p[i].tat;
        avgWT += p[i].wt;
        printf("%s\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    printf("\nContext Switches: %d\n", csCount);
    printf("Total Context Switch Time: %d\n", csTime);
    printf("CPU Efficiency: %.2f%%\n",
           (100.0 * (time - csTime)) / time);
    printf("Average TAT: %.2f\n", avgTAT / n);
    printf("Average WT: %.2f\n", avgWT / n);

    return 0;
}
