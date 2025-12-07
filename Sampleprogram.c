#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
    if(argc < 4){
       printf("Usage: %s num1 num2 num3\n",argv[0]);
     return 1;
}

int a = atoi(argv[1]);
int b = atoi(argv[2]);
int c = atoi(argv[3]);


int max = a;
if(b > max) max = b;
if(c > max) max = c;

printf("Greatest number: %d\n",max);
return 0;
}

