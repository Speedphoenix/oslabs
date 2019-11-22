#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define PERMS 0660

#define READY_1 0x1
#define READY_2 0x2

// (a + b) * (c - d) + (e + f)

// or (e + f) + (a + b) * (c - d)

int main(int argc, char **argv)
{
	int a, b, c, d, e, f;
	int rep;
	int *repptr;
	unsigned char *flagptr;
	int cid1, cid2;
	int memid, flagmemid;

	a = 4;
	b = 8;
	c = 14;
	d = 2;
	e = 54;
	f = 20;

	flagmemid = shmget(IPC_PRIVATE, sizeof(char), IPC_CREAT | PERMS);
	flagptr = (unsigned char *)shmat(flagmemid, NULL, 0);
	*flagptr = 0;

	memid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | PERMS);
	repptr = (int *)shmat(memid, NULL, 0);

	if (fork() == 0)
	{
		// (a + b) * (c - d)

		if (fork() == 0)
		{
			*repptr = c - d;
			*flagptr |= READY_1;
			exit(0);
		}
		else
		{
			rep = a + b;
			while (!(*flagptr & READY_1))
				(void)NULL;
			*repptr = rep * *repptr;
			*flagptr |= READY_2;
			exit(0);
		}
	}
	else
	{
		rep = e + f;
		while (!(*flagptr & READY_2))
			(void)NULL;
		rep += *repptr;
		shmctl(memid, IPC_RMID, NULL);
	}

	printf("result is %d\n", rep);

	return (0);
}

