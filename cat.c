#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

//Starts here
typedef struct flags
{
  int number;
  int s;
  }
  flags;

int ERR_STATUS=0;



void displayline_no(int line_no)
{
  int no_len = floor(log10(line_no)) + 1;
  int padsize = 6 - no_len;

  for (int i = 0; i < padsize; i++)
    printf(" ");

  printf("%d  ", line_no);
}

void stdin_loop(int *line_no, flags flag)
{
  while(1)
  {
    char string[200];
    scanf("%s", string);
    if (flag.number)
      displayline_number((intptr_t) ++(*line_no));
    printf("%s%s\n", string, flag.s ? "$" : "");

    if (feof(stdin))
      break;
  }
}

int main(int ar, char *argv[])
{
  char *paths[ar];
  int path_cnt=0;
  flags flag ={ 0, 0 };

  for (int i = 1; i < ar; i++)
  {
    char *arg = argv[a];

    // Double dash keyword options
    if (strlen(arg) > 2 && arg[0] == '-' && arg[1] == '-')
    {
      if (strcmp(arg, "--show-ends") == 0) flag.s = 1;
      else if (strcmp(arg, "--number") == 0) flag.number = 1;
      else
      {
        fprintf(stderr, "cat: unrecognized option '%s'\n", arg);
        exit(EXIT_FAILURE);
      }
      continue;
    }

    // Single dash keyword ltr options
    if (strlen(arg) > 1 && arg[0] == '-')
    {
      for (int j = 1; j < strlen(arg); j++)
      {
        char ltr = arg[j];
        if (ltr == 'E') flag.s = 1;
        else if (ltr == 'n') flag.number = 1;
        else
        {
          fprintf(stderr, "cat: invalid option -- '%c'\n", ltr);
          exit(EXIT_FAILURE);
        }
      }
      continue;
    }

    // This argument is not an option/flag its a path to be cat'ed
    paths[path_cnt++]=arg;
  }

  int line_no=0;
  int n_line=1;

  // Copy standard input to standard output
  if (path_cnt == 0)
  {
    stdin_loop(&line_number, flag);
    return 0;
  }

  for (int i = 0; i < path_cnt; i++)
  {
    char *path = paths[i];

    if (strcmp(path, "-") == 0)
    {
      stdin_loop(&line_number, flag);
      continue;
    }

    struct stat path_stat;
    stat(path, &path_stat);
    if (!S_ISREG(path_stat.st_mode))
    {
      fprintf(stderr, "cat: %s: Is a directory\n", path);
      ERR_STATUS = 1;
      continue;
    }

    FILE *fd = fopen(path, "r");
    if (fd == NULL)
    {
      fprintf(stderr, "cat: %s: No such file or directory\n", path);
      ERR_STATUS = 1;
      continue;
    }

    int c;

    while ((c = fgetc(fd)) && !feof(fd))
    {
      if (n_line && flag.number)
        displayline_number(++line_number);

      n_line=0;

      if (c == '\n')
      {
        n_line = 1;
        if (flag.s)
          printf("$");
      }

      printf("%c", c);
    }

    fclose(fd);
  }

  return ERR_STATUS;
}
