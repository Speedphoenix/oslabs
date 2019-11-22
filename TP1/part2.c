#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int	main(int argc, char** argv)
{
	int	fret;
	int	fret2;
	int val;

	val = 5;
	printf("The process started\n");
	fret = fork();
	if (fret)
	{
		printf("I'm the parent (I'm %d, child is %d, and my parent is %d)\n",
			getpid(), fret, getppid());
		val++;
		printf("Parent side val: %d\n", val);
	}
	else if (fret == -1)
	{
		printf("error happened (I'm %d my parent is %d, error is %d)\n",
			getpid(), getppid(), errno);
	}
	else
	{
		printf("I'm the child (I'm %d my parent is %d)\n",
			getpid(), getppid());
		val--;
		printf("Child side val: %d\n", val);
	}
	printf("End val: %d\n", val);

	fret2 = fork();

	printf("\n");
	sleep(2);

	if (fret)
	{
		if (fret2)
			printf("I am the parent (%d), children are %d and %d\n", getpid(),
				fret, fret2);
		else
			printf("I am the second child (%d) of the parent (%d)\n", getpid(),
				getppid());
	}
	else
	{
		if (fret2)
			printf("I am the first child (%d), parent is %d, and second"
				" child is %d\n", getpid(), getppid(), fret2);
		else
			printf("I am the second child (%d), my parent is %d\n", getpid(),
				getppid());
	}
	return 0;
}
