#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include "sig.h"

int ID;
int count = 0;
pid_t parent;

void ready_handler(int sig)
{
    printf("compute(%d): Sending SIGUSR2 to parent process.\n", ID);
    kill(parent, SIGUSR2);
}

void compute_handler(int sig)
{
    printf("compute(%d): SIGUSR1 received.\n", ID);
    int i, j, k;
    for (k = 0; k < (ID + 1) * INC; k++)
    {
        for (i = 1; i < k; i++)
        {
            for (j = 1; j < i; j++)
            {
                if (i * i + j * j == k * k)
                {
                    count++;
                }
            }
        }
    }
    printf("compute(%d): For interval [%d,%d): count = %d\n", ID, ID * INC, (ID + 1) * INC, count);
    kill(parent, SIGUSR2);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <ID>\n", argv[0]);
        exit(1);
    }

    ID = atoi(argv[1]);
    parent = getppid();

    signal(SIGUSR2, ready_handler);
    signal(SIGUSR1, compute_handler);

    printf("compute(%d): Ready.\n", ID);
    kill(parent, SIGUSR2);

    while (1)
    {
        sleep(1);
    }

    return 0;
}