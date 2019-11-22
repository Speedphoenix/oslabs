#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/times.h>
#include <time.h>
#include <sys/resource.h>


// Colors to liven up the terminal
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define RESET "\x1B[0m"

// these are 80 underscores
#define SEPP CYN "_________________________________________" \
				 "_______________________________________" RESET

#define REPEAT 5000

#define NOTHING ' '

typedef struct	s_oper
{
	int rep;
	struct s_oper *a;
	struct s_oper *b;
	char op;
}				t_oper;

// (a + b) – [(c * d) / (e - f)] + (g + h)
//	  1	   1 	 2	  2	   4	 1	  3

void *doop(void *fargs);

void *doop(void *fargs)
{
	pthread_t thread1, thread2;
	int iret1, iret2;

	t_oper *args = (t_oper *)fargs;
	if (args->op == NOTHING)
		return NULL;

	iret1 = pthread_create(&thread1, NULL, doop, (void *)args->a);
	iret2 = pthread_create(&thread2, NULL, doop, (void *)args->b);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	switch (args->op)
	{
	case '+':
		args->rep = args->a->rep + args->b->rep;
		break;

	case '-':
		args->rep = args->a->rep - args->b->rep;
		break;

	case '*':
		args->rep = args->a->rep * args->b->rep;
		break;

	case '/':
		args->rep = args->a->rep / args->b->rep;
		break;

	case '%':
		args->rep = args->a->rep % args->b->rep;
		break;
	}
	return NULL;
}

// if an argument is passed, it means the result should not be given
int main(int argc, char **argv)
{
	int i;
	struct tms before, after;
	struct rusage preusage, postusage;
	clock_t utime, stime;
	int tickpsec;

	tickpsec = sysconf(_SC_CLK_TCK);

	printf(YEL "The execution is repeated %d times" RESET "\n", REPEAT);
	times(&before);
	getrusage(RUSAGE_CHILDREN, &preusage);

	for (i = 0; i < REPEAT; i++)
	{
		t_oper vars[15];

		vars[0].rep = 4 + i; // a
		vars[0].op = NOTHING;
		vars[1].rep = 8; // b
		vars[1].op = NOTHING;
		vars[2].rep = 14 + i; // c
		vars[2].op = NOTHING;
		vars[3].rep = 2; // d
		vars[3].op = NOTHING;
		vars[4].rep = 54; // e
		vars[4].op = NOTHING;
		vars[5].rep = 47; // f
		vars[5].op = NOTHING;
		vars[6].rep = 76; // g
		vars[6].op = NOTHING;
		vars[7].rep = 32; // h
		vars[7].op = NOTHING;

		// a + b
		vars[8].a = &(vars[0]);
		vars[8].b = &(vars[1]);
		vars[8].op = '+';

		// c * d
		vars[9].a = &(vars[2]);
		vars[9].b = &(vars[3]);
		vars[9].op = '*';

		// e - f
		vars[10].a = &(vars[4]);
		vars[10].b = &(vars[5]);
		vars[10].op = '-';

		// g + h
		vars[11].a = &(vars[6]);
		vars[11].b = &(vars[7]);
		vars[11].op = '+';

		// the middle division
		vars[12].a = &(vars[9]);
		vars[12].b = &(vars[10]);
		vars[12].op = '/';

		// the big substraction
		vars[13].a = &(vars[8]);
		vars[13].b = &(vars[12]);
		vars[13].op = '-';

		// the big addition
		vars[14].a = &(vars[13]);
		vars[14].b = &(vars[11]);
		vars[14].op = '+';

		doop(&(vars[14]));

//		if (argc <= 1)
//			printf("result is %d\n", vars[14].rep);
	}

	getrusage(RUSAGE_CHILDREN, &postusage);
	times(&after);

	utime = after.tms_cutime + after.tms_utime - (before.tms_cutime + before.tms_utime);
	stime = after.tms_cstime + after.tms_stime - (before.tms_cstime + before.tms_stime);

	printf("\n" RED "THREADS" RESET "\n\n");
	printf("User time:\t%ld ticks\t%f s\n", utime, ((double) utime) / tickpsec);
	printf("Syst time:\t%ld ticks\t%f s\n", stime, ((double) stime) / tickpsec);
	printf("Full time:\t%ld ticks\t%f s\n", utime + stime,
			((double) (utime + stime)) / tickpsec);
	printf("\n");
	printf("I/O block:\t%ld Input\t%ld Output\n",
		postusage.ru_inblock - preusage.ru_inblock,
		postusage.ru_oublock - preusage.ru_oublock);

	printf("Context switches:\t%ld\n",
		postusage.ru_nvcsw - preusage.ru_nvcsw);

	printf("Involuntary context switches:\t%ld\n",
		postusage.ru_nivcsw - preusage.ru_nivcsw);
	return 0;
}
