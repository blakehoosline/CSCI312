#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include "sig.h"

int ready_count = 0;
int done_count = 0;
pid_t pid[MAXCP];

void ready_handler(int sig)
{
	printf("controller: SIGUSR2 received.\n");
	ready_count++;
	if (ready_count == MAXCP)
	{
		printf("controller: All compute processes are ready.\n");
		for (int i = 0; i < MAXCP; i++)
		{
			kill(pid[i], SIGUSR1);
		}
	}
}

void done_handler(int sig)
{
	printf("controller: SIGUSR2 received.\n");
	done_count++;
	if (done_count == MAXCP)
	{
		printf("controller: All compute processes are done.\n");
		for (int i = 0; i < MAXCP; i++)
		{
			kill(pid[i], SIGUSR1);
		}
	}
}

int main()
{
	signal(SIGUSR2, ready_handler);
	signal(SIGUSR2, done_handler);

	printf("controller: Starting...\n");

	for (int i = 0; i < MAXCP; i++)
	{
		pid[i] = fork();
		if (pid[i] == -1)
		{
			perror("fork");
			exit(1);
		}
		if (pid[i] == 0)
		{
			char id_str[10];
			sprintf(id_str, "%d", i);
			execl("./compute", "compute", id_str, NULL);
			perror("exec");
			exit(1);
		}
	}

	while (ready_count < MAXCP)
	{
		sleep(1);
	}
	printf("controller: Starting computation...\n");

	time_t start_time = time(NULL);

	while (done_count < MAXCP)
	{
		sleep(1);
	}

	time_t end_time = time(NULL);
	printf("controller: Computation time: %ld seconds.\n", end_time - start_time);

	for (int i = 0; i < MAXCP; i++)
	{
		int status;
		waitpid(pid[i], &status, 0);
		printf("controller: Child process (%d) terminated with exit status: %d\n", pid[i], status);
	}

	printf("controller: Complete.\n");

	return 0;
}