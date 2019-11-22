#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// ps aux | more

int main(int argc, char **argv)
{
	int pipefd[2];
	int f1;
	int fork_ret[3];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
		dup2(pipefd[0], 0);
		dup2(pipefd[1], 1);

	fork_ret[0] = fork();
	if (fork_ret[0] == 0)
	{
		close(pipefd[1]); /* Close unused write end */
		execl("/bin/more", "more", (char *)NULL);
	}
	else
	{
		close(pipefd[0]); /* Close unused read end */
		if (fork() == 0)
			execl("/bin/ps", "ps", "aux", (char *)NULL);
		else
			waitpid(fork_ret[0], NULL, 0);
	}
	return (0);
}
