#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int id;
    int C, T, phi;
} Task;

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int lcm(int a, int b) {
    return (a * b) / gcd(a, b);
}

int main() {
    int n;
    printf("Enter number of tasks: ");
    scanf("%d", &n);

    Task t[n];
    for (int i=0; i<n; i++) {
        t[i].id = i+1;
        printf("T%d Computation Time (C): ", i+1);
        scanf("%d", &t[i].C);
        printf("T%d Period (T): ", i+1);
        scanf("%d", &t[i].T);
        t[i].phi = 0; // assume offset = 0
    }

    // Utilization test
    double U = 0;
    for (int i=0; i<n; i++) U += (double)t[i].C / t[i].T;
    double bound = n * (pow(2.0, 1.0/n) - 1.0);

    printf("\n--- RMS Analysis ---\n");
    printf("Total Utilization U = %.3f\n", U);
    printf("Liu-Layland Bound = %.3f\n", bound);
    if (U <= bound) printf("Schedulability Test: Guaranteed schedulable by bound.\n");
    else if (U <= 1.0) printf("Schedulability Test: Not guaranteed (need RTA).\n");
    else { printf("Schedulability Test: Definitely not schedulable (U>1).\n"); return 0; }

    // Response Time Analysis
    int schedulable = 1;
    for (int i=0; i<n; i++) {
        int Rprev = t[i].C, R = Rprev;
        while (1) {
            int interference = 0;
            for (int j=0; j<i; j++) {
                interference += (int)ceil((double)Rprev / t[j].T) * t[j].C;
            }
            R = t[i].C + interference;
            if (R == Rprev) break;
            if (R > t[i].T) break;
            Rprev = R;
        }
        printf("Task T%d: Worst-case Response Time = %d, Deadline = %d -> %s\n",
               t[i].id, R, t[i].T, (R <= t[i].T) ? "Schedulable" : "Missed");
        if (R > t[i].T) schedulable = 0;
    }
    if (schedulable) printf("All tasks schedulable under RTA.\n");
    else printf("Some tasks miss deadlines under RTA.\n");

    // Simulation (hyperperiod)
    int H = t[0].T;
    for (int i=1; i<n; i++) H = lcm(H, t[i].T);
    printf("\n--- Simulation up to Hyperperiod %d ---\n", H);

    int time = 0;
    int remaining[n], deadline[n], active[n];
    for (int i=0;i<n;i++) { remaining[i]=0; deadline[i]=0; active[i]=0; }

    printf("Timeline: ");
    while (time < H) {
        // release jobs
        for (int i=0;i<n;i++) {
            if (time % t[i].T == t[i].phi) {
                remaining[i] = t[i].C;
                deadline[i] = time + t[i].T;
                active[i] = 1;
            }
        }
        // pick highest priority (smallest T)
        int idx=-1;
        for (int i=0;i<n;i++) {
            if (active[i] && remaining[i]>0) {
                if (idx==-1 || t[i].T < t[idx].T) idx=i;
            }
        }
        if (idx!=-1) {
            printf("|%d-T%d", time, t[idx].id);
            remaining[idx]--;
            if (remaining[idx]==0) active[idx]=0;
        } else {
            printf("|%d-IDLE", time);
        }
        // deadline check
        for (int i=0;i<n;i++) {
            if (active[i] && time+1 > deadline[i]) {
                printf(" (Miss: T%d deadline at %d)", t[i].id, deadline[i]);
                active[i]=0;
            }
        }
        time++;
    }
    printf("|%d\n", H);

    return 0;
}
