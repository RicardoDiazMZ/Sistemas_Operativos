#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS_NO 3

int partes[THREADS_NO][3] = {{1, 3}, {4, 6}, {7, 9}};

int factorial(int n) 
{
    int result = 1;
    for(int i = 2; i <= n; i++) 
    {
        result *= i;
    }
    return result;
}

void *threadFact(void *args) 
{
    int *data = (int *) args;
    int result = factorial(data[1]) / factorial(data[0] - 1);
    printf("result of thread %d = %d\n", *data, result);
    return (void *) result;
}

int main() 
{
    int n = 9;
    int result = 1;
   
    pthread_t threads[THREADS_NO];

    for(int i = 0; i < THREADS_NO; i++) 
    {
        int *data = malloc(2 * sizeof(int));
        data[0] = partes[i][0];
        data[1] = partes[i][1];
        pthread_create(&threads[i], NULL, threadFact, data);
    }

    for(int i = 0; i < THREADS_NO; i++) 
    {
        int resultThread = 1;
        pthread_join(threads[i], (void **) &resultThread);
    }
    
    return 0;
}
