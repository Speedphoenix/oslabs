#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define PERMS 0660

int my_system(char *cmd);

// ((a+b)-(c+d))

void doop(int *repptr, int a, int b, char op)
{
	switch (op)
	{
		case '+':
			*repptr = a + b;
			break;

		case '-':
			*repptr = a - b;
			break;

		case '*':
			*repptr = a * b;
			break;

		case '%':
			*repptr = a % b;
			break;

		default:
			break;
	}
}

// launches to do the operation asynchronously, the pointer that
// will eventually contain the result is *rep
// this returns the id of the memory segment so it can e freed later
int launchop(int **rep, int a, int b, char op)
{
	int id;

	id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | PERMS);

	*rep = (int *)shmat(id, NULL, 0);

	if (fork() == 0)
	{
		// do the operation, put the result in *rep, then quit
		doop(*rep, a, b, op);
		exit(0);
	}
	return id;
}

int main(int argc, char **argv)
{
	int a, b, c, d;
	int rep;
	int *repptr;
	int id;

	a = 4;
	b = 8;
	c = 14;
	d = 2;

	// launch on the small operations to do
	id = launchop(&repptr, c, d, '+');

	// in the meantime do the other
	rep = a + b;

	// wait for that operation from before to finish
	wait(NULL);
	//now the result from before is *repptr

	rep -= *repptr;

	//we don't need the memory segment anymore. We can clear it now
	shmctl(id, IPC_RMID, NULL);

	printf("The result is %d\n", rep);

	return (0);
}
