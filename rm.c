#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>



typedef struct flags

{
  int b;
  int a;
} flags;

int ERR_STATUS = 0;

int main(int ar, char *argv[])
{
  char *paths[ar];
  int path_count = 0;
  flags flag = { 0, 0 };

  for (int i = 1; i < ar; i++)
  {
    char *arg = argv[i];

    // Double dash keyword options
    if (strlen(arg) > 2 && arg[0] == '-' && arg[1] == '-')
    {
      if (strcmp(arg, "--b") == 0) flag.b = 1;
      else if (strcmp(arg, "--a") == 0) flag.a = 1;
      else
      {
        fprintf(stderr, "rm: unrecognized option '%s'\n", arg);
        exit(EXIT_FAILURE);
      }
      continue;
    }

    // Single dash keyword letter options
    if (strlen(arg) > 1 && arg[0] == '-')
    {
      for (int j = 1; j < strlen(arg); j++)
      {
        char letter = arg[j];
        if (letter == 'v') flag.b = 1;
        else if (letter == 'f') flag.a = 1;
        else
        {
          fprintf(stderr, "rm: invalid option -- '%c'\n", letter);
          exit(EXIT_FAILURE);
        }
      }
      continue;
    }

    // This argument is not an option/flag its a path to be cat'ed
    paths[path_count++]=arg;
  }

  if (path_count == 0 && !flag.a)
  {
    fprintf(stderr, "rm: missing operand\n");
    exit(1);
  }

  for (int a = 0; a < path_count; a++)
  {
    char *path = paths[a];
    if (unlink(path) == -1)
    {
      switch (errno)
      {
        case EACCES:
          fprintf(
            stderr,
            "rm: cannot remove '%s': Permission denied\n",
            path
          );
          break;

        case EISDIR:
          fprintf(
            stderr,
            "rm: cannot remove '%s': Is a directory\n",
            path
          );
          break;

        case ENOENT:
          if (!flag.a)
            fprintf(
              stderr,
              "rm: cannot remove '%s': No such file or directory\n",
              path
            );
      }

      ERR_STATUS = 1;
      continue;
    }

    if (flag.b)
      printf("Removed '%s'\n", path);
  }

  return 0;
}
