#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXP 50

typedef struct {
    int id;
    int at;
    int bt;
    int rem;
    int ct;
    int started;
} proc;

int all_done(proc p[], int n){
    for(int i=0;i<n;i++) if(p[i].rem>0) return 0;
    return 1;
}

int main(){
    int n;
    printf("Enter number of processes: ");
    if(scanf("%d",&n)!=1) return 0;
    proc p[MAXP];
    for(int i=0;i<n;i++){
        p[i].id = i+1;
        printf("P%d arrival time and burst time: ", i+1);
        scanf("%d %d",&p[i].at,&p[i].bt);
        p[i].rem = p[i].bt;
        p[i].ct = -1;
        p[i].started = 0;
    }
    int C;
    printf("Context switch time C: ");
    scanf("%d",&C);

    int time = 0;
    int last_running = -1; // index of process
    int cs_count = 0;
    int total_cs_time = 0;
    int busy_time = 0;

    // Gantt entry array: we'll store segments as strings for clarity
    char gantt[10000]; gantt[0]=0;
    char seg[200];

    while(!all_done(p,n)){
        // find ready processes
        int idx = -1;
        for(int i=0;i<n;i++){
            if(p[i].at<=time && p[i].rem>0){
                if(idx==-1) idx=i;
                else if(p[i].rem < p[idx].rem) idx=i;
                else if(p[i].rem == p[idx].rem && p[i].at < p[idx].at) idx=i;
            }
        }

        if(idx==-1){
            // idle: advance time to next arrival
            int next_at = 1<<30;
            for(int i=0;i<n;i++) if(p[i].rem>0 && p[i].at>time) if(p[i].at < next_at) next_at = p[i].at;
            if(next_at==1<<30) break;
            // add idle interval to gantt
            sprintf(seg, "%d-Idle-%d-", time, next_at);
            strcat(gantt, seg);
            time = next_at;
            last_running = -1;
            continue;
        }

        // decide preemption: if currently running and a new process just arrived that could preempt
        if(last_running==-1){
            // CPU was idle -> starting process idx, but context switch occurs before starting
            if(C>0){
                sprintf(seg, "%d-CS-%d-", time, time+C);
                strcat(gantt, seg);
                time += C;
                total_cs_time += C;
                cs_count++;
            }
            // begin execution
            last_running = idx;
            p[idx].started = 1;
        } else {
            // there's a current process last_running
            // check if idx is different and can preempt under condition rem(curr) > rem(new) + C
            if(idx != last_running){
                if(p[last_running].rem > p[idx].rem + C){
                    // preempt => do CS, then switch
                    // But first we will record running interval if any (we always simulate unit-by-unit so there may be zero)
                    sprintf(seg, "%d-CS-%d-", time, time+C);
                    strcat(gantt, seg);
                    time += C;
                    total_cs_time += C;
                    cs_count++;
                    last_running = idx;
                    if(!p[idx].started) p[idx].started = 1;
                } else {
                    // do not preempt -> continue running last_running
                    idx = last_running;
                }
            } else {
                idx = last_running;
            }
        }

        // run chosen process for 1 time unit (we simulate unit granularity)
        sprintf(seg, "%d-P%d-%d-", time, p[idx].id, time+1);
        strcat(gantt, seg);
        time += 1;
        p[idx].rem -= 1;
        busy_time += 1;
        last_running = idx;
        if(p[idx].rem == 0){
            p[idx].ct = time;
            last_running = -1; // will cause cs before next start if next starts
        }
    }

    // print gantt (trim trailing '-')
    int L = strlen(gantt);
    if(L>0 && gantt[L-1]=='-') gantt[L-1]=0;
    printf("\nGantt Chart:\n%s\n\n", gantt);

    // metrics
    printf("P\tAT\tBT\tCT\tTAT\tWT\n");
    double sumT=0,sumW=0;
    for(int i=0;i<n;i++){
        int tat = p[i].ct - p[i].at;
        int wt = tat - p[i].bt;
        sumT += tat; sumW += wt;
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, p[i].bt, p[i].ct, tat, wt);
    }
    printf("\nContext switches: %d\nTotal CS time: %d\nTotal busy time: %d\nTotal timeline length: %d\n", cs_count, total_cs_time, busy_time, time);
    double eff = ((double)busy_time / (double)time) * 100.0;
    printf("CPU Efficiency: %.2f%%\n", eff);

    return 0;
}
