#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void *task1(void *arg);
void *task2(void *arg);

void *task1(void *arg)
{
	int *val;

	val = (int *)arg;
	(*val)++;
	return NULL;
}

void *task2(void *arg)
{
	int *val;

	val = (int *)arg;
	(*val)--;
	return NULL;
}

int main()
{
	int i = 65;
	pthread_t thread1, thread2;

	pthread_create(&thread1, NULL, task1, &i);
	pthread_create(&thread2, NULL, task2, &i);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	printf("%d\n", i);
	return 0;
}
