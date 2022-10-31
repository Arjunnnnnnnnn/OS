#include <stdlib.h>
#include <string.h>
#include "../global.h"
#include <limits.h>
#include <stdio.h>


WD working_dirs;

int pwd(int ar, char *argv[])
{
  int phy_p = 0;

  for (int a = 0; a < ar; a++)
  {
    char *arg = argv[a];
    // Single dash keyword letter options
    if (strlen(arg) > 1 && arg[0] == '-')
    {
      for (int b = 1; b < strlen(arg); b++)
      {
        char letter = arg[b];
        if (letter == 'P') phy_p = 1;
        else if (letter == 'L') phy_p = 0;
        else
        {
          fprintf(stderr, "p8sh: pwd: -%c: invalid option\n", letter);
          return 1;
        }
      }
    }
  }

  if (phy_p)
    printf("%s\n", realpath(working_dirs.current, NULL));
  else
    printf("%s\n", working_dirs.current);

  return 0;
}
