#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t s1, s2, s3;

typedef struct s_op
{
	int a;
	int b;
	int *dest;
	sem_t *sem;
}t_op;

// (a + b) * (c - d) * (e + f)
// (5 + 3) * (12 - 1) * (6 + 1)

void *addition(void *args)
{
	t_op *what = (t_op *)args;

	*(what->dest) = what->a + what->b;
	sem_post(what->sem);
	return NULL;
}

void *substraction(void *args)
{
	t_op *what = (t_op *)args;

	*(what->dest) = what->a - what->b;
	sem_post(what->sem);
	return NULL;
}

void *full(void *args)
{
	int *tab = (int *)args;
	int rep;

	sem_wait(&s1);
	sem_wait(&s2);
	rep = tab[0] * tab[1];
	sem_wait(&s3);
	rep *= tab[2];

	printf("the result is %d\n", rep);
	return NULL;
}

int main()
{
	pthread_t thread1, thread2, thread3, thread4;
	int tab[3];
	t_op ops[3];

	ops[0].a = 5;
	ops[0].b = 3;
	ops[0].dest = &(tab[0]);
	ops[0].sem = &s1;

	ops[1].a = 12;
	ops[1].b = 1;
	ops[1].dest = &(tab[1]);
	ops[1].sem = &s2;

	ops[2].a = 6;
	ops[2].b = 1;
	ops[2].dest = &(tab[2]);
	ops[2].sem = &s3;

	sem_init(&s1, 0, 0);
	sem_init(&s2, 0, 0);
	sem_init(&s3, 0, 0);

	printf("Doing (5 + 3) * (12 - 1) * (6 + 1)\n");

	pthread_create(&thread1, NULL, addition, &(ops[0]));
	pthread_create(&thread2, NULL, substraction, &(ops[1]));
	pthread_create(&thread3, NULL, addition, &(ops[2]));
	pthread_create(&thread4, NULL, full, tab);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);
	return 0;
}
