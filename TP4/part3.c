#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
	int i, j;
	char buf[100];
	// open stdin in nonblocking mode
	// fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
	for (i = 0; i < 10; i++)
	{
		int nb;
		nb = read(STDIN_FILENO, buf, 100);
		printf("nwrites = %d\terror = %d\n", nb, errno);
		for (j = 0; j < nb; j++)
		{
			printf("%d\n", buf[j]);
		}
	}
	return (0);
}
