#include <stdio.h>
int main() {
    int n, i;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    int at[n], bt[n], rt[n], ct[n], tat[n], wt[n];
    for(i = 0; i < n; i++) {
        printf("Enter arrival time and burst time for process %d: ", i+1);
        scanf("%d %d", &at[i], &bt[i]);
        rt[i] = bt[i];
    }
    int time = 0, completed = 0;
    while(completed < n) {
        int idx = -1;
        int min_rt = 1e9;
        for(i = 0; i < n; i++) {
            if(at[i] <= time && rt[i] < min_rt && rt[i] > 0) {
                min_rt = rt[i];
                idx = i;
            }
        }
        if(idx != -1) {
            rt[idx]--;
            time++;
            if(rt[idx] == 0) {
                completed++;
                ct[idx] = time;
                tat[idx] = ct[idx] - at[idx];
                wt[idx] = tat[idx] - bt[idx];
            }
        } else {
            time++;
        }
    }
    printf("P\tAT\tBT\tCT\tTAT\tWT\n");
    for(i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", i+1, at[i], bt[i], ct[i], tat[i], wt[i]);
    }
    return 0;
}
