#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id, at, bt, pr;
    int ct, tat, wt, done;
} Process;

double priority_weight(int pr) {
    return 1.0 + (pr - 1) * 0.2;   // lower pr = higher priority
}

int main() {
    int n, starvation_threshold;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];
    int maxBT = 0;

    for (int i=0; i<n; i++) {
        p[i].id = i+1;
        printf("P%d Arrival Time: ", i+1);
        scanf("%d", &p[i].at);
        printf("P%d Burst Time: ", i+1);
        scanf("%d", &p[i].bt);
        printf("P%d Priority (1=High): ", i+1);
        scanf("%d", &p[i].pr);
        p[i].done = 0;
        if (p[i].bt > maxBT) maxBT = p[i].bt;
    }

    starvation_threshold = maxBT * 2;
    printf("Starvation Threshold = %d\n", starvation_threshold);

    int completed = 0, t = 0;
    printf("\nExecution Log:\n");

    while (completed < n) {
        int idx = -1;
        double best_ratio = -1.0;
        int anti_starvation = 0;

        // find eligible processes
        for (int i=0; i<n; i++) {
            if (!p[i].done && p[i].at <= t) {
                int wt = t - p[i].at;
                // check starvation
                if (wt > starvation_threshold) {
                    anti_starvation = 1;
                }
            }
        }

        for (int i=0; i<n; i++) {
            if (!p[i].done && p[i].at <= t) {
                int wt = t - p[i].at;
                double ratio;
                if (anti_starvation) {
                    ratio = (double)(wt + p[i].bt) / p[i].bt; // standard RR
                } else {
                    ratio = (double)(wt + p[i].bt) / (p[i].bt * priority_weight(p[i].pr));
                }
                if (ratio > best_ratio) {
                    best_ratio = ratio;
                    idx = i;
                }
            }
        }

        if (idx == -1) { t++; continue; } // no process ready

        // schedule process
        printf("Time %d: Running P%d (BT=%d, PR=%d) [%s]\n", 
               t, p[idx].id, p[idx].bt, p[idx].pr, 
               (best_ratio && best_ratio>0 && anti_starvation) ? "Anti-starvation" : "Highest ARR");

        t += p[idx].bt;
        p[idx].ct = t;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        p[idx].done = 1;
        completed++;
    }

    printf("\nProcess AT  BT  PR  CT  TAT WT\n");
    float avgTAT=0, avgWT=0;
    for (int i=0; i<n; i++) {
        avgTAT += p[i].tat;
        avgWT += p[i].wt;
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].at, p[i].bt, p[i].pr,
               p[i].ct, p[i].tat, p[i].wt);
    }
    printf("\nAvg TAT = %.2f, Avg WT = %.2f\n", avgTAT/n, avgWT/n);

    return 0;
}
