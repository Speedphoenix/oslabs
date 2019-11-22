#define _POSIX_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

#define REPEAT 100000

int main(int argc, char **argv)
{
	int i, j;
	int fork_ret;
	struct tms before, after;
	struct rusage preusage, postusage;
	clock_t utime, stime;
	int tickpsec;

	tickpsec = sysconf(_SC_CLK_TCK);

	printf("Clock ticks per sec:%d\n", tickpsec);
	printf("Ticks per sec asking a different source: %ld\n\n", CLOCKS_PER_SEC);

	printf(YEL "Executions are repeated %d times" RESET "\n\n", REPEAT);
	times(&before);
	getrusage(RUSAGE_CHILDREN, &preusage);

	for (j = 0; j < REPEAT; j++)
	{
		/*
		char str[200];
		FILE * fic = fopen("tmp.txt", "w");
		fscanf(fic, "%s", str);
		fclose(fic);
		*/
		printf("hmméi\n");
	}

	getrusage(RUSAGE_CHILDREN, &postusage);
	times(&after);

	utime = after.tms_cutime - before.tms_cutime;
	stime = after.tms_cstime - before.tms_cstime;

	printf("\n" RED "%s" RESET "\n\n", "WRITE");
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


	if (i != argc - 1)
		printf("\n\n" SEPP "\n\n");
	return 0;
}
