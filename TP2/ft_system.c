#include <unistd.h>
#include <sys/wait.h>

int my_system(char *cmd)
{
	int fork_ret;
	int termination_status;

	termination_status = 0;
	fork_ret = fork();
	if (fork_ret == 0)
	{
		// this is the tway used in the man page of system()
		// this could also have been done by taking the arguments
		// separated by spaces and put them in an array or char *,
		// then use execv, execvp or execvpe instead of execl
		execl("/bin/sh", "sh", "-c", cmd, (char *)NULL);
	}
	else if (fork_ret == -1)
		return -1;
	else
	{
		wait(&termination_status);
	}
	return termination_status;
}
