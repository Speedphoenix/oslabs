#define _POSIX_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/times.h>
#include <time.h>
#include <sys/resource.h>

// Colors to liven up the terminal
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define RESET "\x1B[0m"

// these are 80 underscores
#define SEPP CYN "_________________________________________" \
				 "_______________________________________" RESET

#define REPEAT 5000

#define ES(x) fprintf(stderr, #x "\n");
#define E(x) fprintf(stderr, #x " (%d): %d\n", __LINE__, (x));

#define PERMS 0660

// (a + b)  + (– [(c * d) / (e - f)]) + (g + h)
//	  1	    1  2	 2	  2	   4	  2	   3

// if an argument is passed, it means the result should not be given
int main(int argc, char **argv)
{
	int i;
	struct tms before, after;
	struct rusage preusage, postusage;
	clock_t utime, stime;
	int tickpsec;
	int *repptr1, *repptr2;
	int memid1, memid2;

	tickpsec = sysconf(_SC_CLK_TCK);

	printf(YEL "The execution is repeated %d times" RESET "\n", REPEAT);
	times(&before);
	getrusage(RUSAGE_CHILDREN, &preusage);

	memid1 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | PERMS);
	repptr1 = (int *)shmat(memid1, NULL, 0);

	memid2 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | PERMS);
	repptr2 = (int *)shmat(memid2, NULL, 0);

	for (i = 0; i < REPEAT; i++)
	{
		int a, b, c, d, e, f, g, h;
		int rep;
		int cid[3];

		a = 4;
		b = 8;
		c = 14;
		d = 2;
		e = 54;
		f = 47;
		g = 76;
		h = 32;

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
		}

		//if (argc <= 1)
		//	printf("result is %d\n", rep);
	}
	shmctl(memid1, IPC_RMID, NULL);
	shmctl(memid2, IPC_RMID, NULL);


	getrusage(RUSAGE_CHILDREN, &postusage);
	times(&after);

	utime = after.tms_cutime + after.tms_utime - (before.tms_cutime + before.tms_utime);
	stime = after.tms_cstime + after.tms_stime - (before.tms_cstime + before.tms_stime);

	printf("\n" RED "PROCESSES" RESET "\n\n");
	printf("User time:\t%ld ticks\t%f s\n", utime, ((double) utime) / tickpsec);
	printf("Syst time:\t%ld ticks\t%f s\n", stime, ((double) stime) / tickpsec);
	printf("Full time:\t%ld ticks\t%f s\n", utime + stime,
			((double) (utime + stime)) / tickpsec);
	printf("\n");
	printf("I/O block:\t%ld Input\t%ld Output\n",
		postusage.ru_inblock - preusage.ru_inblock,
		postusage.ru_oublock - preusage.ru_oublock);

	printf("Context switches:\t%ld\n",
		postusage.ru_nvcsw - preusage.ru_nvcsw);

	printf("Involuntary context switches:\t%ld\n",
		postusage.ru_nivcsw - preusage.ru_nivcsw);
	return (0);
}
