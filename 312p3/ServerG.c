#include "soc.h"
#include "shared.h"

int main(int argc, char *argv[])
{
    // Server Related
    int err, cSocket, gameNum, received = 1;
    char Buf[BUFL];
    char sBuf[BUFL];

    // Game Related Variables
    int serverPick, clientPick, numGames, winNumGames, winner, gameDone = 0;
    int sWins = 0, cWins = 0;

    if (argc == 3)
    {
        cSocket = atoi(argv[1]);
        gameNum = atoi(argv[2]);
    }
    else
    {
        exit(20);
    }
    printf("socServerG:Game %d: Client socket %d\n", gameNum, cSocket);

    while (received > 0 & gameDone == 0)
    {
        numGames = genRandGame();
        winNumGames = (numGames / 2) + 1;
        // Send initial instructions
        sprintf(sBuf,
                "This is a game of Rock, Paper, Scissors.\nWe will play the "
                "best %d of %d\n",
                winNumGames, numGames);
        err = send(cSocket, sBuf, strlen(sBuf) + 1, 0);
        // Client acknowledge
        received = recv(cSocket, Buf, BUFL, 0);
        // Each round
        while (1)
        {
            // Send Move options
            sprintf(sBuf,
                    "Choose Move:\n 1) Rock\n 2) Paper\n 3) Scissors\n --> ");
            err = send(cSocket, sBuf, strlen(sBuf) + 1, 0);

            // Receive input from client
            received = recv(cSocket, Buf, BUFL, 0);
            printf("socServerG:Game %d: %s received\n", gameNum, Buf);

            // Convert client pick to integer
            clientPick = atoi(Buf) - 1;
            // Generate pick for Server
            serverPick = getServerPlay();
            printf("socServerG:Game %d: Client Picked %s, Server Picked % s\n",
                   gameNum, getIdMove(clientPick), getIdMove(serverPick));
            // Repeat moves for client
            sprintf(sBuf, "\n\nYou chose %s\nServer chose %s\n",
                    getIdMove(clientPick), getIdMove(serverPick));
            err = send(cSocket, sBuf, strlen(sBuf) + 1, 0);
            received = recv(cSocket, Buf, BUFL, 0);

            // Determine Winner of round
            winner = determineWinner(serverPick, clientPick);
            switch (winner)
            {
            case 0:
                // Tie game
                sprintf(sBuf,
                        "Tie\nThe Server has %d wins, Player has %d wins\n",
                        sWins, cWins);
                break;
            case 1:
                // Server wins
                sWins += 1;
                sprintf(
                    sBuf,
                    "Server wins\nThe Server has %d wins, Player has %d wins\n",
                    sWins, cWins);
                break;
            case 2:
                // Client wins
                cWins += 1;
                sprintf(
                    sBuf,
                    "Player wins\nThe Server has %d wins, Player has %d wins\n",
                    sWins, cWins);
                break;
            default:
                break;
            }
            if (cWins == winNumGames)
            {
                printf("socServerG:Game %d: Player wins\n", gameNum);
                sprintf(sBuf, "%s\nPlayer wins!", sBuf);
            }
            else if (sWins == winNumGames)
            {
                printf("socServerG:Game %d: Server wins\n", gameNum);
                sprintf(sBuf, "%s\nServer wins.\nBetter luck next time!",
                        sBuf);
            }
            sndString(cSocket, sBuf);
            // Client acknowledge
            received = recv(cSocket, Buf, BUFL, 0);
            if (cWins == winNumGames | sWins == winNumGames)
            {
                gameDone = 1;
                break;
            }
        }
        sndString(cSocket, "Press 1 to exit\n");
        exit(0);
    }
    close(cSocket);
    printf("socServerG:Game %d: connection %d closed\n", gameNum, cSocket);
}
