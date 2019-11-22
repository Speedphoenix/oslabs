#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t svim, sgedit;

void *launch_gedi(void *args)
{
	system("gedit");
	sem_post(&sgedit);
	return NULL;
}

void *launch_vim(void *args)
{
	sem_wait(&sgedit);
	system("vim");
	sem_post(&svim);
	return NULL;
}

void *launch_fire(void *args)
{
	sem_wait(&svim);
	system("firefox");
	return NULL;
}

int main()
{
	pthread_t thread1, thread2, thread3;
	sem_init(&svim, 0, 0);
	sem_init(&sgedit, 0, 0);

	printf("Opening in order:\ngedit\nvim\nfirefox\n");

	pthread_create(&thread2, NULL, launch_fire, NULL);
	pthread_create(&thread1, NULL, launch_vim, NULL);
	pthread_create(&thread3, NULL, launch_gedi, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	return 0;
}
