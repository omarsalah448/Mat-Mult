#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct input_args{
    int i;
    int j;
} input_args;

void *fun1(void *args){
    input_args *arg = (input_args *)args;
    printf("%d\n%d\n\n", arg->i, arg->j);
    int *res = (int *)malloc(sizeof(int));
    *res = arg->i + arg->j;
    return res;
}

int main(void){
    pthread_t thread1;
    input_args *args = (input_args *)malloc(sizeof(input_args));
    args->i = 17;
    args->j = 20;
    pthread_create(&thread1, NULL, fun1, args);
    int *ok = 0;
    pthread_join(thread1, (void *)&ok);
    printf("%d\n", *ok);
    return 0;
}