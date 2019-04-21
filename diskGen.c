#include <stdlib.h>
#include <stdio.h>


int main(){

    int time = 0;
    FILE *fp = fopen("file","w");
    
    for(int i = 0; i < 1000; i++){
        time += rand() % 11;
        fprintf(fp,"%d %d %d\n", time, rand() % (1 << 20), rand() % 100);
    }
    return 0;
}