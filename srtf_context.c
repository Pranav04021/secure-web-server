#include <stdio.h>
#include <limits.h>

int main() {
    int n, cs_overhead;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int at[n], bt[n], rt[n], ct[n], tat[n], wt[n];
    int completed = 0, time = 0, prev = -1;
    int context_switches = 0;
    float total_tat = 0, total_wt = 0, busy_time = 0;

    printf("Enter context switch overhead (in time units): ");
    scanf("%d", &cs_overhead);

    printf("Enter arrival and burst times:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d Arrival Time: ", i + 1);
        scanf("%d", &at[i]);
        printf("P%d Burst Time: ", i + 1);
        scanf("%d", &bt[i]);
        rt[i] = bt[i];
    }

    while (completed != n) {
        int shortest = -1, min_rt = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (at[i] <= time && rt[i] > 0 && rt[i] < min_rt) {
                min_rt = rt[i];
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

        rt[shortest]--;
        time++;
        busy_time++;  // CPU is executing a process
        prev = shortest;

        if (rt[shortest] == 0) {
            completed++;
            ct[shortest] = time;
            tat[shortest] = ct[shortest] - at[shortest];
            wt[shortest] = tat[shortest] - bt[shortest];
            total_tat += tat[shortest];
            total_wt += wt[shortest];
        }
    }

    float total_time = time;
    float cs_time = context_switches * cs_overhead;
    float efficiency = (busy_time / total_time) * 100;

    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", i + 1, at[i], bt[i], ct[i], tat[i], wt[i]);
    }

    printf("\nTotal Context Switches: %d", context_switches);
    printf("\nTotal Context Switch Time: %.2f", cs_time);
    printf("\nCPU Efficiency: %.2f%%", efficiency);
    printf("\nAverage Turnaround Time: %.2f", total_tat / n);
    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);

    return 0;
}
