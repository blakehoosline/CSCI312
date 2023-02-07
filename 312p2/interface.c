#include "interface.h"

int main(void)
{
  // pipe to the child
  int toChild[2];
  // pipe to the parent
  int toParent[2];
  // for error checking
  int err;
  // to process the ID
  int childPid;
  // db exit status
  int status;

  // open toChild pipe file descriptor
  err = pipe(toChild);
  if (err == -1)
  {
    printf("Error on pipe creation: %d\n", errno);
    exit(EXIT_FAILURE);
  }
  // open toParent pipe file descriptor
  err = pipe(toParent);
  if (err == -1)
  {
    printf("Error on pipe creation: %d\n", errno);
    exit(EXIT_FAILURE);
  }
  // fork to get new process
  childPid = fork();
  if (childPid == -1)
  {
    printf("parent: fork failed, errno = %d\n", errno);
    exit(1);
  }
  else if (childPid == 0)
  {
    close(toChild[1]);
    close(toParent[0]);
    char char_fd_to_child[10];
    sprintf(char_fd_to_child, "%d", toChild[0]);
    char char_fd_to_parent[10];
    sprintf(char_fd_to_parent, "%d", toParent[1]);

    // run the db
    // first argument is program name and then pipe to file descriptors
    err = execl("./db", "db", char_fd_to_child, char_fd_to_parent, (char *)NULL);
    if (err == -1)
    {
      printf("parent: execl failed, errno = %d\n", errno);
      exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
  }
  close(toChild[0]);
  close(toParent[1]);

  char inp[20];
  while (1)
  {
    fprintf(stderr, "Input command: (account,id | date,mddyy | exit) :\n");
    scanf("%s", &inp);
    // write the input over to db
    err = write(toChild[1], inp, strlen(inp) + 1);
    if (err == -1)
    {
      printf("Error on write to pipe: %d\n", errno);
      exit(EXIT_FAILURE);
    }

    // read response from pipe after the command was sent
    int n;
    char buf[BUFFER_SIZE];
    int i = 0;
    char temp;
    n = read(toParent[0], &buf, BUFFER_SIZE);
    if (n == -1)
    {
      printf("Error on pipe read: %d\n", errno);
      exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
    {
      if (buf[i] == '\0')
      {
        fprintf(stderr, "\n");
      }
      else
      {
        fprintf(stderr, "%c", buf[i]);
      }
    }
    if (n < 0)
    {
      printf("pipe read failed, errno = %d\n", errno);
      exit(EXIT_FAILURE);
    }
    buf[n] = '\0';
    fflush(stderr);
    sleep(1);
    if (!strncmp(inp, "exit", strlen("exit")))
    {
      break;
    }
  }
  // wait for the child to finish
  if (waitpid(childPid, &status, 0) == -1)
  {
    perror("waitpid failed");
    return EXIT_FAILURE;
  }
  // detect the child exit status
  if (WIFEXITED(status))
  {
    const int es = WEXITSTATUS(status);
    fprintf(stderr, "interface:  child process exit status = %d\n", es);
  }
  fprintf(stderr, "interface:  complete\n");
  return EXIT_SUCCESS;
}