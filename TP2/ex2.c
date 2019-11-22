#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define PERMS 0660

// (a + b) * (c - d) + (e + f)

// or (e + f) + (a + b) * (c - d)

int main(int argc, char **argv)
{
	int a, b, c, d, e, f;
	int rep;
	int *repptr;
	int cid1, cid2;
	int memid;

	a = 4;
	b = 8;
	c = 14;
	d = 2;
	e = 54;
	f = 20;

	memid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | PERMS);
	repptr = (int *)shmat(memid, NULL, 0);

	cid1 = fork();
	if (cid1 == 0)
	{
		// (a + b) * (c - d)

		cid2 = fork();
		if (cid2 == 0)
		{
			*repptr = c - d;
			exit(0);
		}
		else
		{
			rep = a + b;
			waitpid(cid2, NULL, 0);
			*repptr = rep * *repptr;
			exit(0);
		}
	}
	else
	{
		rep = e + f;
		waitpid(cid1, NULL, 0);
		rep += *repptr;
		shmctl(memid, IPC_RMID, NULL);
	}

	printf("result is %d\n", rep);

	return (0);
}

