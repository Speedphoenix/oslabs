#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char** argv)
{
	int	fret;

	printf("The process started\n");
	fret = fork();
	if (fret)
	{
		printf("I'm the parent (I'm %d, child is %d, and my parent is %d)\n",
			getpid(), fret, getppid());
	}
	else
	{
		printf("I'm the child (I'm %d my parent is %d)\n",
			getpid(), getppid());
	}
	return 0;
}
