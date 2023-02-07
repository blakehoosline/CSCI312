#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define STDOUT 1
#define STDERR 2
#define DB_FILE "./accountData"
// maximum records
#define MAX_RECORDS 10000
// maximum buffer size
#define BUFFER_SIZE 10000

#define ACCOUNT "account,"
#define DATE "date,"
#define EXIT "exit"

// records struct
typedef struct record
{
  char id[15];
  int checkNumber;
  int date;
  float amount;
} record;

// function prototypes
char *concat(const char *s1, const char *s2);
char *save_res_to_arr(int print_until, struct record records_to_print[]);
void write_with_syscall(int fd, char *str, int len);
void read_records(char *str, int flags);
char *getId(char *inp);
char *getDate(char *inp);
