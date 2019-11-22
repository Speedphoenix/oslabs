#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define PERMS 0660

// (a + b)  + (– [(c * d) / (e - f)]) + (g + h)
//	  1	    1  2	 2	  2	   4	  2	   3

#define NOTHING ' '


// if an argument is passed, it means the result should not be given
int main(int argc, char **argv)
{
	int a, b, c, d, e, f, g, h;
	int rep;
	int *repptr1, *repptr2;
	int cid[3];
	int memid1, memid2;

	a = 4;
	b = 8;
	c = 14;
	d = 2;
	e = 54;
	f = 47;
	g = 76;
	h = 32;

	memid1 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | PERMS);
	repptr1 = (int *)shmat(memid1, NULL, 0);

	memid2 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | PERMS);
	repptr2 = (int *)shmat(memid2, NULL, 0);

	cid[0] = fork();
	if (cid[0] == 0)
	{
		cid[1] = fork();
		if (cid[1] == 0)
		{
			*repptr2 = g + h;
			exit(0);
		}
		else
		{
			cid[2] = fork();

			if (cid[2] == 0)
			{
				*repptr1 = e - f;
				exit(0);
			}
			else
			{
				rep = c * d;
				waitpid(cid[2], NULL, 0);
				rep = rep / *repptr1;

				rep *= -1;

				waitpid(cid[1], NULL, 0);
				rep += *repptr2;
				*repptr1 = rep;
				exit(0);
			}
		}
	}
	else
	{
		rep = a + b;
		waitpid(cid[0], NULL, 0);
		rep += *repptr1;
		shmctl(memid1, IPC_RMID, NULL);
		shmctl(memid2, IPC_RMID, NULL);
	}

	if (argc <= 1)
		printf("result is %d\n", rep);
	return (0);
}
