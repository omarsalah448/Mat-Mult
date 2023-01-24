#include<stdlib.h>
#include<pthread.h>
#include <unistd.h>

#define Item(X, I, J, NCOL)      X[((J) + (I) * (NCOL))]

// struct to send the arguments
typedef struct arguments
{
    int* A;
    int* B;
    int* C;
    int i;
    int j;
    int m;
    int n;
} arguments;

/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult(int* A, int* B, int* C, int l, int m, int n)
{
    for(int i=0; i<l; i++)
    {
        for(int j=0; j<n; j++)
        {
            int sum = 0;
            for(int k=0; k<m; k++)
                sum += Item(A, i, k, m) * Item(B, k, j, n);
            Item(C, i, j, n) = sum;
        }
    }
}

// thread function to compute one element
void *computeElement(void *input_args)
{
    // variable to be returned
    int sum = 0;
    arguments *args = (arguments *)input_args;
    for(int k=0; k<args->m; k++)
        sum += Item(args->A, args->i, k, args->m) * Item(args->B, k, args->j, args->n);
    Item(args->C, args->i, args->j, args->n) = sum;
}

// Version 1)The computation of each element of the output matrix happens in a thread
void matmult_v1(int* A, int* B, int* C, int l, int m, int n)
{
    // variable to be returned
    int *sum = 0;
    // create a thread
    pthread_t threads[l*n];
    // send the arguments
    arguments args[l*n];
    for(int i=0; i<l; i++)
    {
        for(int j=0; j<n; j++)
        {
            args[j + i*n].A = A;
            args[j + i*n].B = B;
            args[j + i*n].C = C;
            args[j + i*n].m = m;
            args[j + i*n].n = n;
            args[j + i*n].i = i;
            args[j + i*n].j = j;
            // start the thread
            pthread_create(&threads[j + i*n], NULL, &computeElement, &args[j + i*n]);
        }
    }
    for(int i=0; i<l; i++)
    {
        for(int j=0; j<n; j++)
        {
            // wait for it to finish
            pthread_join(threads[j + i*n], NULL);
        }
    }
    
}

// thread function to compute one row
void *computeRow(void *input_args)
{
    arguments *args = (arguments *)input_args;
    for(int j=0; j<args->n; j++)
    {
        int sum = 0;
        for(int k=0; k<args->m; k++)
            sum += Item(args->A, args->i, k, args->m) * Item(args->B, k, j, args->n);
        Item(args->C, args->i, j, args->n) = sum;
    }
}

// Version 2)The computation of each row of the output matrix happens in a thread
void matmult_v2(int* A, int* B, int* C, int l, int m, int n)
{
    // create a thread
    pthread_t threads[l];
    arguments args[l];
    for(int i=0; i<l; i++){
        args[i].A = A;
        args[i].B = B;
        args[i].C = C;
        args[i].m = m;
        args[i].n = n;
        args[i].i = i;
        // start the thread
        pthread_create(&threads[i], NULL, computeRow, &args[i]);
    }
    for(int i=0; i<l; i++){
        // wait for it to finish
        pthread_join(threads[i], NULL);
    }
}
