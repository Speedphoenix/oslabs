#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sa, sb, sc;

typedef struct s_vals
{
	int a;
	int b;
	int c;
}t_vals;

// a += 10
// c--
void *task1(void *arg);

// a *= 5
// b -= 3
void *task2(void *arg);

// b <<= 2
// c++
void *task3(void *arg);

void *task1(void *arg)
{
	t_vals *vals;

	vals = (t_vals *)arg;

	printf("t1 on\n");

	sem_wait(&sa);
	usleep(2000); // 2 ms
	sem_wait(&sc);
	printf("t1 start\n");

	sleep(1);

	vals->a += 10;
	vals->c--;

	sem_post(&sa);
	sem_post(&sc);
	printf("t1 end\n");
	return NULL;
}

void *task2(void *arg)
{
	t_vals *vals;

	vals = (t_vals *)arg;
	printf("t2 on\n");

	sem_wait(&sb);
	usleep(2000); // 2 ms
	sem_wait(&sa);

	printf("t2 start\n");

	sleep(1);

	vals->a *= 5;
	vals->b -= 3;

	sem_post(&sb);
	sem_post(&sa);
	printf("t2 end\n");
	return NULL;
}

void *task3(void *arg)
{
	t_vals *vals;

	vals = (t_vals *)arg;
	printf("t3 on\n");

	sem_wait(&sc);
	usleep(2000); // 2 ms
	sem_wait(&sb);

	printf("t3 start\n");

	sleep(1);

	vals->b <<= 2;
	vals->c++;

	sem_post(&sb);
	sem_post(&sc);
	printf("t3 end\n");
	return NULL;
}

int main()
{
	t_vals values = {5, 12, 16};
	pthread_t thread1, thread2, thread3;

	sem_init(&sa, 0, 1);
	sem_init(&sb, 0, 1);
	sem_init(&sc, 0, 1);

	pthread_create(&thread1, NULL, task1, &values);
	pthread_create(&thread2, NULL, task2, &values);
	pthread_create(&thread3, NULL, task3, &values);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	printf("%d, %d, %d\n", values.a, values.b, values.c);
	return 0;
}
