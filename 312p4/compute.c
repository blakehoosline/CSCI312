#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "synchronizer.h"

int id;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <id>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    id = atoi(argv[1]);
    signal(SIGUSR1, compute_handler);
    signal(SIGUSR2, ready_handler);
    while (1)
    {
        pause();
    }
    return 0;
}

void ready_handler(int sig)
{
    report_ready(getpid());
}

void compute_handler(int sig)
{
    int a, b, c, count = 0;
    for (c = 0; c < (id + 1) * INC; c++)
    {
        for (a = 1; a <= c; a++)
        {
            for (b = 1; b <= c; b++)
            {
                if (a * a + b * b == c * c)
                {
                    count++;
                }
            }
        }
    }
    report_done(getpid());
}

void report_ready(int pid)
{
    printf("Compute process %d is ready.\n", pid);
    kill(getppid(), SIGUSR2);
}

void report_done(int pid)
{
    printf("Compute process %d is done.\n", pid);
    kill(getppid(), SIGUSR2);
    pause();
    kill(getppid(), SIGUSR1);
}
