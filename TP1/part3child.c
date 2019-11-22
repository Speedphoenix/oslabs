#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int i;
	int *a;

	printf("I'm a child (I think). I'm %d parent is %d,\n", getpid(), getppid());
	printf("I have %d params:\n", argc);
	for (i = 0; i < argc; i++)
	{
		printf("\t%s\n", argv[i]);
	}
	if (argc >= 2)
	{
		printf("pointer to a seems to be %s\n", argv[1]);
		sscanf(argv[1], "%p", &a);
		printf("pointer to a  (actual one) seems to be %p\n", a);
		printf("trying to show a: ");
		fflush(stdout);
		printf("%d\n", *a);
		printf("\ntrying to change a\n");
		*a += 5;
		printf("new a: %d\n", *a);
	}
	return (0);
}
