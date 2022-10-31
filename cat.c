#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct flags
{
    int number;
    int s;
}flags;

int ERR_STATUS = 0;



void stdin_loop(int *line_number, flags flag)
{
  while(1)
  {
    char string[200];
    scanf("%s", string);
    if (flag.number)
      displayline_number((intptr_t) ++(*line_number));
    printf("%s%s\n", string, flag.s ? "$" : "");

    if (feof(stdin))
      break;
  }
}

void displayline_number(int line_number)
{
    int number_length = floor(log10(line_number)) + 1;
    int size = 6 - number_length;

    for (int i = 0; i < size; i++)
      printf(" ");

    printf("%d  ", line_number);
}

int main(int ar, char *argv[])
{
  char *paths[ar];
  int path_count = 0;
  flags flag = { 0, 0 };

  for (int a = 1; a < ar; a++)
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

    // Single dash keyword letter options
    if (strlen(arg) > 1 && arg[0] == '-')
    {
      for (int b = 1; b < strlen(arg); b++)
      {
        char letter = arg[b];
        if (letter == 'E') flag.s = 1;
        else if (letter == 'n') flag.number = 1;
        else
        {
          fprintf(stderr, "cat: invalid option -- '%c'\n", letter);
          exit(EXIT_FAILURE);
        }
      }
      continue;
    }

    // This argument is not an option/flag its a path to be cat'ed
    paths[path_count++] = arg;
  }

  int line_number = 0;
  int new_line = 1;

  // Copy standard input to standard output
  if (path_count == 0)
  {
    stdin_loop(&line_number, flag);
    return 0;
  }

  for (int a = 0; a < path_count; a++)
  {
    char *path = paths[a];

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
      if (new_line && flag.number)
        print_line_number(++line_number);

      new_line = 0;

      if (c == '\n')
      {
        new_line = 1;
        if (flag.s)
          printf("$");
      }

      printf("%c", c);
    }

    fclose(fd);
  }

  return ERR_STATUS;
}
