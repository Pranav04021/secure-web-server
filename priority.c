#include <stdio.h>
int main() {
    int n, i, j;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    int at[n], bt[n], priority[n], ct[n], tat[n], wt[n], completed[n];
    for(i = 0; i < n; i++) {
        printf("Enter arrival time, burst time, and priority for process %d: ", i+1);
        scanf("%d %d %d", &at[i], &bt[i], &priority[i]);
        completed[i] = 0;
    }
    int time = 0, count = 0;
    while(count < n) {
        int idx = -1;
        int highest = 1e9;
        for(i = 0; i < n; i++) {
            if(at[i] <= time && completed[i] == 0 && priority[i] < highest) {
                highest = priority[i];
                idx = i;
            }
        }
        if(idx != -1) {
            time += bt[idx];
            ct[idx] = time;
            tat[idx] = ct[idx] - at[idx];
            wt[idx] = tat[idx] - bt[idx];
            completed[idx] = 1;
            count++;
        } else {
            time++;
        }
    }
    printf("P\tAT\tBT\tPriority\tCT\tTAT\tWT\n");
    for(i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\t%d\n", i+1, at[i], bt[i], priority[i], ct[i], tat[i], wt[i]);
    }
    return 0;
}
