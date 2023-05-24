#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "synchronizer.h"

pid_t pids[MAXCP];
int ready_count = 0, done_count = 0;
clock_t start_time;

int main()
{
    int i, status;
    signal(SIGUSR2, ready_handler);
    signal(SIGUSR2, done_handler);
    signal(SIGUSR1, print_handler);
    start_time = clock();
    for (i = 0; i < MAXCP; i++)
    {
        if ((pids[i] = fork()) == 0)
        {
            compute(i);
        }
    }
    synchronize();
    printf("All compute processes are ready.\n");
    kill(pids[0], SIGUSR1);
    while (done_count < MAXCP)
    {
        pause();
    }
    print_results();
    for (i = 0; i < MAXCP; i++)
    {
        kill(pids[i], SIGTERM);
        waitpid(pids[i], &status, 0);
        printf("Compute process %d exited with status %d.\n", pids[i], WEXITSTATUS(status));
    }
    printf("Controller terminated.\n");
    return 0;
}

void ready_handler(int sig)
{
    ready_count++;
    if (ready_count == MAXCP)
    {
        kill(pids[0], SIGUSR1);
    }
}

void done_handler(int sig)
{
    done_count++;
    if (done_count == MAXCP)
    {
        pause();
        kill(getpid(), SIGUSR1);
    }
}

void print_handler(int sig)
{
    int i;
    for (i = 0; i < MAXCP; i++)
    {
        kill(pids[i], SIGUSR1);
    }
}

void synchronize()
{
    while (ready_count < MAXCP)
    {
        pause();
    }
}

void print_results()
{
    int i, j;
    double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    printf("Elapsed time: %.2f seconds.\n", elapsed_time);
    for (i = 0; i < MAXCP; i++)
    {
        printf("Results from compute process %d:\n", pids[i]);
        for (j = 0; j < (i + 1) * INC; j += INC / 10)
        {
            printf("%5d: %5d\n", j, (i + 1) * (j / INC + 1));
        }
        printf("\n");
    }
}