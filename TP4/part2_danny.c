#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// ps aux | more

int main(int argc, char **argv)
{
	int pipefd[2];
	int f1;
	int cpid;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	  cpid = fork();

  if (cpid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (cpid == 0) { /* Child reads from pipe */
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[1]); /* Close unused write end */
     if(fork() ==0){
      execl("/bin/more", "more", (char *)NULL);
     }
    else{
      close(pipefd[0]);
      exit(EXIT_SUCCESS);
     }
  }
  else
  { /* We are going to execute "ps more" wich will send the info to */
      close(pipefd[0]); /* Close unused read end */
      dup2(pipefd[1], STDOUT_FILENO);
    if(fork() == 0)// to avoid a zombie when execl()
    {
      execl("/bin/ps", "ps", "aux", (char *)NULL);
    }
    else{
	   waitpid(cpid, NULL, 0);
      exit(EXIT_SUCCESS);
    }
	}
}
