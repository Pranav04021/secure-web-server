#include <stdio.h>
#include <limits.h>

#define MAX 100

typedef struct {
    int pid, at, bt, rt, ct, tat, wt;
} Process;

typedef struct {
    int pid;
    int start;
    int end;
} Gantt;

int main() {
    int n, cs_overhead;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];
    Gantt gantt[MAX];
    int g_index = 0;

    printf("Enter context switch overhead (in time units): ");
    scanf("%d", &cs_overhead);

    printf("Enter arrival and burst times:\n");
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("P%d Arrival Time: ", i + 1);
        scanf("%d", &p[i].at);
        printf("P%d Burst Time: ", i + 1);
        scanf("%d", &p[i].bt);
        p[i].rt = p[i].bt;
    }

    int completed = 0, time = 0, prev = -1;
    int context_switches = 0;
    float total_tat = 0, total_wt = 0, busy_time = 0;

    while (completed != n) {
        int shortest = -1, min_rt = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0 && p[i].rt < min_rt) {
                min_rt = p[i].rt;
                shortest = i;
            }
        }

        if (shortest == -1) {
            time++;
            continue;
        }

        // Context switch overhead
        if (prev != -1 && prev != shortest) {
            time += cs_overhead;
            context_switches++;
        }

        // Gantt chart entry
        if (g_index == 0 || gantt[g_index - 1].pid != p[shortest].pid) {
            gantt[g_index].pid = p[shortest].pid;
            gantt[g_index].start = time;
        }

        p[shortest].rt--;
        time++;
        busy_time++;
        prev = shortest;

        if (p[shortest].rt == 0) {
            completed++;
            p[shortest].ct = time;
            p[shortest].tat = p[shortest].ct - p[shortest].at;
            p[shortest].wt = p[shortest].tat - p[shortest].bt;
            total_tat += p[shortest].tat;
            total_wt += p[shortest].wt;

            gantt[g_index].end = time;
            g_index++;
        }
    }

    float total_time = time;
    float cs_time = context_switches * cs_overhead;
    float efficiency = (busy_time / total_time) * 100;

    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    printf("\nGantt Chart:\n|");
    for (int i = 0; i < g_index; i++) {
        printf(" P%d |", gantt[i].pid);
    }
    printf("\n0");
    for (int i = 0; i < g_index; i++) {
        printf("   %d", gantt[i].end);
    }

    printf("\n\nTotal Context Switches: %d", context_switches);
    printf("\nTotal Context Switch Time: %.2f", cs_time);
    printf("\nCPU Efficiency: %.2f%%", efficiency);
    printf("\nAverage Turnaround Time: %.2f", total_tat / n);
    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);

    return 0;
}
