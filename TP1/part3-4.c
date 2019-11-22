#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char** argv)
{
	int i;

	i = 5;

	if (fork() == 0)
	{
		execl("/usr/lib/firefox/firefox", "/usr/lib/firefox/firefox", (char *)NULL);
		i++; // this will not be done
		printf("%d\n", i); // this will not print
	}
	else
	{
		printf("I am the parent %d, i is %d", getpid(), i);
		// here i is still 5
	}
	return 0;
}
