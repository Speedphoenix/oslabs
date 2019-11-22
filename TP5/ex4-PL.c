#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t sem1, semResult;
int results[3] = {0, 0, 0};
int a = 5, b = 3, c = 12, d = 1, e = 6, f = 1;

// (a + b) * (c - d) * (e + f)
// (5 + 3) * (12 - 1) * (6 + 1)
//fill results tab
void fillTab(int result){
    int i = 0;

    sem_wait(&semResult);
    for(i ; i < sizeof(results); i++)
    {
        if(results[i] == 0)
        {
            results[i] = result;
			break;
        }
    }
    sem_post(&semResult);
}
void *addition53()
{
    //do some math
    int result = 0;
    result = a + b;
    //fill the tab
    fillTab(result);
    //post semaphore to let full() make calculations with results tab
    sem_post(&sem1);
    return NULL;
}

void *addition61()
{
    //do some math
    int result = 0;
    result = e + f;
    //fill the tab
    fillTab(result);
    //post semaphore to let full() make calculations with results tab
    sem_post(&sem1);
    return NULL;
}

void *substraction121()
{
    //do some math
    int result = 0;
    result = c - d;
    //fill the tab
    fillTab(result);
    //post semaphore to let full() make calculations with results tab
    sem_post(&sem1);
    return NULL;
}

void *full()
{
    int rep;

    sem_wait(&sem1);
    sem_wait(&sem1);
    rep = results[0] * results[1];
    sem_wait(&sem1);
    rep *= results[2];

    printf("the result is %d\n", rep);
    return NULL;
}

int main()
{
    pthread_t thread1, thread2, thread3, thread4;

    sem_init(&sem1, 0, 0);
    sem_init(&semResult, 0, 1);

    printf("Doing (5 + 3) * (12 - 1) * (6 + 1)\n");

    pthread_create(&thread1, NULL, addition53, NULL);
    pthread_create(&thread2, NULL, substraction121, NULL);
    pthread_create(&thread3, NULL, addition61, NULL);
    pthread_create(&thread4, NULL, full, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    return 0;
}

