#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define PERMS 0644

int main(int argc, char **argv)
{
	int pipefd[2];
	int fork_ret[3];
	int f1, f2;

	f1 = open("text1", O_RDONLY);
	f2 = open("text2", O_WRONLY | O_CREAT | O_TRUNC, PERMS);
	dup2(f1, pipefd[0]);
	dup2(f2, pipefd[1]);

	pipe(pipefd);
	dup2(pipefd[0], 0);
	dup2(pipefd[1], 1);

	sleep(4);


	return (0);
}
