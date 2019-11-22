#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char** argv)
{
	int a;
	char str[15];
	int *ptr;

	// to try sharing the stack
	ptr = &a;

	// to try sharing the heap
	// ptr = malloc(sizeof(int));

	*ptr = 5;

	str[0] = '\0';

	sprintf(str, "%p", ptr);

	printf("Pointer is %p\n", ptr);

	printf("Process Id is %d\n", getpid());

	if (argc >= 2)
	{
		printf("creating child process %s\n", argv[1]);
		execl(argv[1], argv[1], str, (char *)NULL);
	}
	sleep(2);
	printf("I'm still alive\n");
	return 0;
}
