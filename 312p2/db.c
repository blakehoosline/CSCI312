#include "db.h"

struct record records[MAX_RECORDS];
int len = 0;

// concatenates two strings
char *concat(const char *s1, const char *s2)
{
  char *result = malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

// saves output that's sent over pipe to an array
char *save_res_to_arr(int print_until, struct record records_to_print[])
{
  char *res = malloc(BUFFER_SIZE);
  int count = 0;

  for (int i = 0; i < print_until; i++)
  {
    char char_i[10];
    char char_checkNumber[10];
    char char_date[10];
    char char_amount[10];
    sprintf(char_i, "%d", i);
    sprintf(char_checkNumber, "%d", records_to_print[i].checkNumber);
    sprintf(char_date, "%d", records_to_print[i].date);
    sprintf(char_amount, "%f", records_to_print[i].amount);

    // generate string to be sent over pipe
    res = concat(res, "element = ");
    res = concat(res, char_i);
    res = concat(res, ": id = ");
    res = concat(res, records_to_print[i].id);
    res = concat(res, ", check number = ");
    res = concat(res, char_checkNumber);
    res = concat(res, ", date = ");
    res = concat(res, char_date);
    res = concat(res, ", amount = ");
    res = concat(res, char_amount);
    res = concat(res, "\n");
  }
  return res;
}

// write to a file descriptor
void write_with_syscall(int fd, char *str, int len)
{
  int err = write(fd, str, len);
  if (err < 0)
  {
    printf("write failed, errno = %d\n", errno);
    exit(EXIT_FAILURE);
  }
}

// read the records from a file
void read_records(char *str, int flags)
{
  int n, fd;
  char buf[BUFFER_SIZE];

  // get file descriptor by opening file
  fd = open(str, flags);
  if (fd < 0)
  {
    printf("open failed, errno = %d\n", errno);
    exit(EXIT_FAILURE);
  }

  // read file from the file descriptor
  n = read(fd, buf, BUFFER_SIZE);
  if (n < 0)
  {
    printf("read failed, errno = %d\n", errno);
    exit(EXIT_FAILURE);
  }
  buf[n] = '\0';

  // use whitespace or \n to split up the records
  char *token = strtok(buf, " \n");
  while (token != NULL)
  {
    strcpy(records[len].id, token);

    token = strtok(NULL, " \n");
    records[len].checkNumber = atoi(token);

    token = strtok(NULL, " \n");
    records[len].date = atoi(token);

    token = strtok(NULL, " \n");
    records[len].amount = atof(token);

    token = strtok(NULL, " \n");
    len++;
  }
  close(fd);
}

// extracts record id from command
char *getId(char *inp)
{
  char *res = malloc(strlen(inp));
  res = strtok(inp, ",");
  res = strtok(NULL, ",");
  return res;
}

// extracts date from command
char *getDate(char *inp)
{
  char *res = malloc(strlen(inp));
  res = strtok(inp, ",");
  res = strtok(NULL, ",");
  return res;
}

// main function
int main(int argc, char *argv[])
{
  read_records(DB_FILE, O_RDONLY);
  char *initialRead = save_res_to_arr(len, records);
  write_with_syscall(STDERR, initialRead, strlen(initialRead) + 1);

  // accountData content will be printed on execution
  if (argc == 3)
  {
    while (1)
    { // start listening for commands from interface
      int n;
      char buf[BUFFER_SIZE];
      n = read(atoi(argv[1]), &buf, BUFFER_SIZE);
      if (n < 0)
      {
        printf("pipe open failed, errno = %d\n", errno);
        exit(EXIT_FAILURE);
      }
      buf[n] = '\0';
      // determine type of command
      if (!strncmp(buf, ACCOUNT, strlen(ACCOUNT)))
      { // If command is ACCOUNT
        char *res = getId(buf);
        float accountSum = 0.0;
        // calculate amount for specfic account
        for (int i = 0; i < len; i++)
        {
          // match the id
          if (!strncmp(res, records[i].id, strlen(res)) && (strlen(res) == strlen(records[i].id)))
          {
            // adds to sum each time a match to entered number is found
            accountSum += records[i].amount;
          }
        }
        // save the result as float to print
        char result[100];
        sprintf(result, "%f", (accountSum));
        // if the number entered is not present in the data file, then zero is returned
        int fd = atoi(argv[2]);
        if (fd > 0)
        {
          write_with_syscall(fd, result, strlen(result) + 1);
        }
        else
        {
          perror("Error: invalid file descriptor");
        }
      }
      else if (!strncmp(buf, DATE, strlen(DATE)))
      { // if command is date
        char *res = getDate(buf);
        float dateSum = 0.0;
        // calculate amount for specfic date
        for (int j = 0; j < len; j++)
        {
          int res_length = strlen(res);
          // match the dates
          char date_str[10];
          sprintf(date_str, "%d", records[j].date);
          if (!strncmp(res, date_str, res_length) && (res_length == strlen(date_str)))
          {
            // adds to sum each time a match to entered date is found
            dateSum += records[j].amount;
          }
        }
        // save the result as float to print
        char result[100];
        sprintf(result, "%f", (dateSum));
        int fd = atoi(argv[2]);
        if (fd > 0)
        {
          write_with_syscall(fd, result, strlen(result) + 1);
        }
        else
        {
          perror("Error: invalid file descriptor");
        }
      }
      else if (!strncmp(buf, EXIT, strlen(EXIT)))
      { // if command is exit
        write_with_syscall(atoi(argv[2]), "child process completed", strlen("child process completed") + 1);
        exit(EXIT_SUCCESS);
      }
      else
      {
        write_with_syscall(atoi(argv[2]), "unknown error", strlen("unknown error") + 1);
      }
    }
  }
}