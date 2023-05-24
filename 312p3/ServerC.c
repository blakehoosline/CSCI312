#include "soc.h"
#include "shared.h"

int main(int argc, char *argv[])
{
    int err, id, sSocket, cSocket, gamesCounter = 0;
    struct sockaddr_in sAddr;
    struct sockaddr_in cAddr;
    char Buf[BUFL];
    int cSocLen;

    sSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (sSocket == -1)
    {
        perror("socServerC: socket creation failed\n");
        exit(1);
    }

    memset(&sAddr, 0, sizeof(struct sockaddr_in));
    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(SERVERPORT);
    sAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    err = bind(sSocket, (struct sockaddr *)&sAddr, sizeof(struct sockaddr_in));
    if (err == -1)
    {
        perror("socServerC: bind address to socket failed\n");
        exit(2);
    }

    err = listen(sSocket, 5);
    if (err == -1)
    {
        perror("socServerC: listen failed\n");
        exit(3);
    }

    printf("Server is awaiting connections\n");

    while (1)
    {
        cSocket = accept(sSocket, (struct sockaddr *)&cAddr, &cSocLen);
        if (cSocket == -1)
        {
            perror("socServerC: accept failed\n");
            exit(4);
        }
        gamesCounter += 1;
        printf("Accept Succesful.\n");

        id = fork();
        if (id == 0)
        { // Fork Child
            printf("socServerC: closing socket for child process\n");
            err = close(sSocket);
            if (err == -1)
            {
                perror("socServerG: server socket close failed\n");
                exit(8);
            }

            char cSockString[BUFL];
            char gameNum[BUFL];

            sprintf(cSockString, "%d", cSocket);
            sprintf(gameNum, "%d", gamesCounter);

            printf("Child Created: Socket - %d\n", cSocket);
            err = execl("./ServerG", "ServerG", cSockString, gameNum, (char *)NULL);
            if (err = -1)
            {
                perror("soServerC: exec Failed.\n");
                exit(5);
            }
        }
        else if (id < 0)
        {
            perror("socServerC: Fork Failed.\n");
            exit(5);
        }
        else
        {
            close(cSocket);
            exit(6);
        }
    }
    exit(0);
}
