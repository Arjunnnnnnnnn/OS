#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>


int exit_wrp(int ar, char *argv[])
{
  int code_ex=0;

  if (ar > 0)
  {
    char *arg = argv[0];
    int b = 0;
    while (isdigit(arg[b]))
      b++;

    if (strlen(arg)!=b)
    {
      fprintf(stderr, "p8sh: exit: %s: numeric argument required\n", arg);
      return 1;
    }

    code_ex = atoi(arg);
  }

  if (ar > 1)
  {
    fprintf(stderr, "p8sh: exit: too many arguments\n");
    return 1;
  }

  exit(code_ex);
}
