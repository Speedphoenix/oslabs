#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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
	t_oper vars[15];

	vars[0].rep = 4; // a
	vars[0].op = NOTHING;
	vars[1].rep = 8; // b
	vars[1].op = NOTHING;
	vars[2].rep = 14; // c
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

	if (argc <= 1)
		printf("result is %d\n", vars[14].rep);
	return 0;
}
