#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t s;

void *task1(void *arg);
void *task2(void *arg);

void *task1(void *arg)
{
	int *val;
	int a;

	val = (int *)arg;

	sem_wait(&s);
	a = *val;
	sleep(1);
	a++;
	*val = a;
	sem_post(&s);
	return NULL;
}

void *task2(void *arg)
{
	int *val;
	int a;

	val = (int *)arg;

	sem_wait(&s);
	a = *val;
	sleep(1);
	a--;
	*val = a;
	sem_post(&s);
	return NULL;
}

int main()
{
	int i = 65;
	pthread_t thread1, thread2, thread3, thread4;

	sem_init(&s, 0, 1);

	pthread_create(&thread1, NULL, task1, &i);
	pthread_create(&thread2, NULL, task2, &i);
	pthread_create(&thread3, NULL, task1, &i);
	pthread_create(&thread4, NULL, task1, &i);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);
	printf("%d\n", i);
	return 0;
}
