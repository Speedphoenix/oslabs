#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char** argv)
{
	int	i;

	i = 5;
	if (fork() == 0)
	{
		// I'm the parent
		i++;
	}
	else
	{
		// I'm the child
		sleep(3);
		printf("%d\n", i); // This prints 5 because the data is duplicated on fork(),
							// but not shared anymore.
	}
	return 0;
}
