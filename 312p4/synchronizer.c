#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include "synchronizer.h"

void compute(int id)
{
    int i, j, count;
    double dt;
    signal(SIGUSR1, compute_handler);
    signal(SIGTERM, terminate_handler);
    printf("Compute process %d initialized.\n", getpid());
    pause();
    printf("Compute process %d started.\n", getpid());
    clock_t start_time = clock();
    for (i = 0; i < (id + 1) * INC; i++)
    {
        count = 0;
        for (j = 0; j < i; j++)
        {
            if ((j * j) % (i + 1) == 1)
            {
                count++;
            }
        }
    }
    dt = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    printf("Compute process %d done in %.2f seconds.\n", getpid(), dt);
    kill(getppid(), SIGUSR2);
    pause();
    printf("Compute process %d printing results.\n", getpid());
    for (i = 0; i < (id + 1) * INC; i += INC / 10)
    {
        printf("%5d: %5d\n", i, (id + 1) * (i / INC + 1));
    }
    printf("\n");
    exit(0);
}

void compute_handler()
{
    printf("Compute process %d received SIGUSR1 signal.\n", getpid());
    pause();
}

void terminate_handler()
{
    printf("Compute process %d received SIGTERM signal.\n", getpid());
    exit(0);
}