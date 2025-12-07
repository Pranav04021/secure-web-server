#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define MAXP 50
typedef struct {
    int id, at, bt, rem, ct;
} P;
int cmp_bt(const void *a,const void *b){ return ((P*)a)->bt - ((P*)b)->bt; }

int main(){
    int n; printf("Enter number of processes: "); if(scanf("%d",&n)!=1) return 0;
    P p[MAXP];
    for(int i=0;i<n;i++){ p[i].id=i+1; printf("P%d AT BT: ",i+1); scanf("%d %d",&p[i].at,&p[i].bt); p[i].rem=p[i].bt; p[i].ct=-1; }
    int time=0, finished=0;
    // Build initial ready queue at time 0
    int in_queue[MAXP]={0};
    int order[1000]; int ord_len=0;
    // find initial ready at time 0
    int initial_count=0;
    for(int i=0;i<n;i++) if(p[i].at<=0) initial_count++;
    int TQ;
    if(initial_count==0){
        // advance to earliest arrival
        int earliest=1<<30;
        for(int i=0;i<n;i++) if(p[i].at < earliest) earliest=p[i].at;
        time = earliest;
        initial_count=0;
        for(int i=0;i<n;i++) if(p[i].at<=time) initial_count++;
    }
    // compute median of BTs of initial ready
    P tmp[MAXP]; int tcnt=0;
    for(int i=0;i<n;i++) if(p[i].at<=time){ tmp[tcnt++]=p[i]; }
    qsort(tmp,tcnt,sizeof(P),cmp_bt);
    if(tcnt%2==1) TQ = tmp[tcnt/2].bt; else TQ = (tmp[tcnt/2 -1].bt + tmp[tcnt/2].bt)/2;
    if(TQ<1) TQ=1;
    printf("Cycle table:\nCycle | TQ | Processes in RQ\n");
    int cycle=1;
    while(finished < n){
        // build ready queue snapshot for this cycle (processes with rem>0 and at<=time)
        int rq[MAXP], rqn=0;
        for(int i=0;i<n;i++) if(p[i].rem>0 && p[i].at<=time) rq[rqn++]=i;
        if(rqn==0){
            // idle until next arrival
            int nxt=1<<30; for(int i=0;i<n;i++) if(p[i].rem>0 && p[i].at>time) if(p[i].at<nxt) nxt=p[i].at;
            time = nxt; continue;
        }
        printf("%d | %d | [", cycle, TQ);
        for(int i=0;i<rqn;i++){ printf("P%d%s", p[rq[i]].id, (i==rqn-1)?"":", "); } printf("]\n");

        // run one cycle: each process in rq gets up to TQ (in order of arrival to ready set)
        for(int i=0;i<rqn;i++){
            int idx = rq[i];
            if(p[idx].rem<=0) continue;
            int exec = (p[idx].rem > TQ) ? TQ : p[idx].rem;
            // if process not yet arrived due to time progression, wait (but we ensured at<=time)
            p[idx].rem -= exec;
            time += exec;
            if(p[idx].rem==0){ p[idx].ct = time; finished++; }
            // allow processes arriving during execution to be added to global readiness (they only affect next cycle)
        }
        // after cycle, recompute TQnext from processes currently in ready queue (rem>0 & at<=time)
        int sum=0, cnt=0;
        for(int i=0;i<n;i++) if(p[i].rem>0 && p[i].at<=time){ sum += p[i].rem; cnt++; }
        int nextTQ = TQ;
        if(cnt>0) nextTQ = (int)ceil((double)sum / (2.0*cnt)); // ceil( average / 2 )
        if(nextTQ < 1) nextTQ = 1;
        TQ = nextTQ;
        cycle++;
    }

    // print Gantt-like timeline (we will provide a simple summary by per-process execution intervals not stored here)
    printf("\nP\tAT\tBT\tCT\tTAT\tWT\n");
    for(int i=0;i<n;i++){
        int tat = p[i].ct - p[i].at;
        int wt = tat - p[i].bt;
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, p[i].bt, p[i].ct, tat, wt);
    }

    return 0;
}

