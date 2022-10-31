#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <limits.h>


typedef struct flags
{
  int b;
  mode_t mode;
} 
flags;

int ERR_STATUS = 0;

void (char *raw_mode, flags flag)
{
  mode_t mode = (mode_t)strtol(raw_mode, NULL, 8);
  if (
      (errno == ERANGE && (mode == LONG_MAX || mode == LONG_MIN)) ||
      (mode == 0))
  {
    fprintf(stderr, "mkdir: invalid mode '%s'", raw_mode);
    exit(EXIT_FAILURE);
  }

  flag.mode = mode;
}

int main(int ar, char *argv[])
{
  char *cwd = argv[0];

  char *paths[ar];
  int path_count = 0;
  flags flag = { 0, 0 };

  for (int a = 1; a < ar; a++)
  {
    char *arg = argv[a];

    // Double dash keyword options
    if (strlen(arg) > 2 && arg[0] == '-' && arg[1] == '-')
    {
      if (strcmp(arg, "--b") == 0) flag.b = 1;
      else
      {
        char keyword[8];
        memcpy(keyword, arg, 7);
        keyword[7] = '\0';

        if (strcmp(keyword, "--mode=") == 0)
        {
          char *raw_mode = &arg[7];
          par_mode(raw_mode, flag);
        }
        else {
          fprintf(stderr, "mkdir: unrecognized option '%s'\n", arg);
          exit(EXIT_FAILURE);
        }
      }
      continue;
    }

    // Single dash keyword letter options
    if (strlen(arg) > 1 && arg[0] == '-')
    {
      if (strcmp(arg, "-v") == 0) flag.b = 1;
      else
      {
        char keyword[4];
        memcpy(keyword, arg, 3);
        keyword[3] = '\0';

        if (strcmp(keyword, "-m=") == 0)
        {
          char *raw_mode = &arg[7];
          par_mode(raw_mode, flag);
        }
        else
        {
          fprintf(stderr, "mkdir: invalid option -- '%c'\n", arg[1]);
          exit(EXIT_FAILURE);
        }
      }
      continue;
    }

    // This argument is not an option/flag its a path to be cat'ed
    paths[path_count++] = arg;
  }

  if (path_count == 0)
  {
    fprintf(stderr, "mkdir: missing operand\n");
    exit(1);
  }

  for (int a = 0; a < path_count; a++)
  {
    char *path = paths[a];

    if (mkdir(path, flag.mode ? flag.mode : 0755) == -1)
    {
      switch (errno)
      {
        case EACCES:
          fprintf(
            stderr,
            "mkdir: cannot create directory '%s': Permission denied\n",
            path
          );
          break;

        case EEXIST:
          fprintf(
            stderr,
            "mkdir: cannot create directory '%s': File exits\n",
            path
          );
          break;

        case ENOENT:
          fprintf(
            stderr,
            "mkdir: cannot create directory '%s': No such file or directory\n",
            path
          );
      }

      ERR_STATUS = 1;
      continue;
    }

    if (flag.b)
      printf("mkdir: created directory '%s'\n", path);
  }

  return 0;
}
