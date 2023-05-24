#include "soc.h"
#include "shared.h"

int main(int argc, char *argv[])
{
    // Variables for Server
    int err, cSocket, validMove, received = 1;
    struct sockaddr_in mainServAddress;
    char Buf[BUFL];
    char sBuf[BUFL];

    // Connection to Server
    memset(&mainServAddress, 0, sizeof(struct sockaddr_in));
    mainServAddress.sin_family = AF_INET;
    mainServAddress.sin_port = htons(SERVERPORT);
    mainServAddress.sin_addr.s_addr = inet_addr(SERVERIP);
    cSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (cSocket == -1)
    {
        perror("socClient: socket creation failed");
        exit(1);
    }

    err = connect(cSocket, (struct sockaddr *)&mainServAddress,
                  sizeof(struct sockaddr_in));
    if (err == -1)
    {
        perror("socClient: connect failed");
        exit(2);
    }
    // Server connection made. Game can be played.

    // Receive game instructions from Server
    err = recv(cSocket, sBuf, BUFL, 0);
    if (err == -1)
    {
        perror("socClient: read failed");
        exit(5);
    }
    printf("%s\n", sBuf);
    // Send blank to acknowledge
    sndString(cSocket, " ");

    // RockPaperScissorGame Code
    while (received > 0)
    {
        // Start of turn
        validMove = 0;
        // Receive round instructions from Server
        err = recv(cSocket, sBuf, BUFL, 0);
        if (err == -1)
        {
            perror("socClient: read failed");
            exit(5);
        }
        while (!(validMove))
        {
            // Print round instructions
            printf("%s", sBuf);
            scanf("%s", Buf);
            // Validate move
            if (atoi(Buf) == 1 | atoi(Buf) == 2 | atoi(Buf) == 3)
            {
                validMove = 1;
            }
            else
            {
                printf("\nInvalid choice, please choose either 1, 2, or 3\n");
            }
        }
        // Sent move
        sndString(cSocket, Buf);

        // Repeat back moves
        err = recv(cSocket, sBuf, BUFL, 0);
        if (err == -1)
        {
            perror("socClient: read failed");
            exit(5);
        }
        printf("%s\n", sBuf);
        // Send blank to acknowledge
        sndString(cSocket, " ");

        // Get results
        err = recv(cSocket, sBuf, BUFL, 0);
        if (err == -1)
        {
            perror("socClient: read failed");
            exit(5);
        }
        printf("%s\n", sBuf);
        // Send blank to acknowledge
        sndString(cSocket, " ");
    }

    exit(0);
}
