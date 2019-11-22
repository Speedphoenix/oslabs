#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 4096

#define PERMS 0644

int ft_strlen(char *str)
{
	int rep;

	rep = 0;
	while (str[rep] != '\0')
		rep++;
	return (rep);
}

void ft_puts(int fd, char *str)
{
	write(fd, str, ft_strlen(str));
}

int main(int argc, char **argv)
{
	int f1, f2;
	int rsize;
	char buf[BUFSIZE];

	f1 = open("text1", O_RDONLY);
	if (f1 == -1)
	{
		ft_puts(2, "Error opening text1\n");
		exit(EXIT_FAILURE);
	}
	f2 = open("text2", O_WRONLY | O_CREAT | O_TRUNC, PERMS);
	if (f2 == -1)
	{
		ft_puts(2, "Error opening text2\n");
		close(f1);
		exit(EXIT_FAILURE);
	}

	// duplicate text2's file descriptor into standard output
	if (dup2(f2, 1) == -1)
	{
		ft_puts(2, "Error dupliating file descriptors text2\n");
		close(f1);
		close(f2);
		exit(EXIT_FAILURE);
	}

	rsize = read(f1, buf, BUFSIZE);
	while (rsize > 0)
	{
		write(f2, buf, rsize);
		rsize = read(f1, buf, BUFSIZE);
	}
	write(f2, "", 1);

	close(f1);
	close(f2);
	return (0);
}
