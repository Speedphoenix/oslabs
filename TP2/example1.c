#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

// the key of the new memory segment. Can be used to later get the segment id again
#define KEY 4567

// The permissions of the new memory segment.
// both the owner and group have read and write access
#define PERMS 0660

int my_system(char *cmd);

int main(int argc, char **argv)
{
	int id;
	int i;
	int *ptr;

	// show the shared memory segments this process has access to
	my_system("ipcs -m");

	// create a new shared memory segment, large enough to accomodate an integer
	id = shmget(KEY, sizeof(int), IPC_CREAT | PERMS);

	// show the shared memory segments this process has access to.
	// there should be one more, and the id of the new one should be `id`
	my_system("ipcs -m");

	// make the pointer ptr point to the start of the new memory segment
	ptr = (int *)shmat(id, NULL, 0);

	// initialize the values.
	// this comes before fork(), so child processes will initially have these values
	*ptr = 54;
	i = 54;

	// fork the process
	if (fork() == 0)
	{
		// this happens if the process is the child

		// modify both values
		// this will be visible from the other process as it is in a shared memory segment
		(*ptr)++;

		// this will only be visible from the child process
		i++;

		//print the values
		printf("Value of *ptr = %d\nValue of i = %d\n", *ptr, i);

		//quit without error
		exit(0);
	}
	else
	{
		// this happens if the process is the parent

		// wait until the child process terminates
		wait(NULL);

		// print the values that the child process might have changed
		// i will still be 54 because it was not changed by the parent
		//	and is not in a shared memory
		// *ptr will have become 55 because the child process changed it and is in shared memory
		printf("Value of *ptr = %d\nValue of i = %d\n", *ptr, i);

		// clear (deallocate) the shared memory segment
		shmctl(id, IPC_RMID, NULL);
	}
	return (0);
}
