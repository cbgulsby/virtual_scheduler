#include <stdio.h>
#include <stdlib.h>

FILE *fp;
long q[10000];
int qi = 0;

typedef struct{
    unsigned int loc     : 30;
    unsigned int proc    : 30;
}rec;

typedef union{
    rec     x;
    long    y;
}rec_un;

void nq(
    long n
){
    q[qi++] = n;
}

long dq(
    long pos
){
    long r = q[pos];
    q[pos] = q[--qi];
    return r;
}

int load_q(
    int time
){
    static int t = -1;   
    static rec_un x;
        int loc, proc;

    if (t > time) time = t;
    while(t <= time){ 
        if (t >= 0)
            nq(x.y); 
        fscanf(fp,"%d%d%d\n",&t, &loc, &proc);
        x.x.loc     = loc;
        x.x.proc    = proc;
        
    }
}

int smallest(
){
    int sp  = 0;

    for(int i = 1; i < qi; i++){
        if(q[i] < q[sp]){
            sp  = i;
        }
    }
    return sp;
}

int largest(
){
    int sp  = 0;

    for(int i = 1; i < qi; i++){
        if(q[i] > q[sp]){
            sp  = i;
        }
    }
    return sp;
}

int process(
    int time
){
    static int dir = 0;

    if(dir){
        while(qi){     
            rec_un y;
            int x = smallest();
            y.y = dq(x);
            time += 5;
            printf("%d %d\n", time, y.x.proc);
        }
    }
    else{
        while(qi){
            rec_un y;
            int x = largest();
            y.y = dq(x);
            time += 5;
            printf("%d %d\n",time, y.x.proc);
        }
    }
    dir ^= 1;
    return time;
}

int main(

){
    int time = 0;

    fp = fopen("file", "r+");
    if(!fp){
        printf("Unable to open file\n");
        exit(1);
    }

    while(!feof(fp)){
        load_q(time);       
        time = process(time);        
    }
}