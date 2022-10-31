#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

//Starts here
typedef struct flags
{
  int no_newline;
  int disable_interpret;
} flags;

int echo(int ar, char *argv[])
{
  char *tokens[ar];
  int token_cnt = 0;
  int parse_opt = 1;
  flags flag = { 0, 0 };

  for (int a = 0; a < ar; a++)
  {
    char *arg = argv[a];

    // Single dash keyword ltr options
    if (parse_opt && strlen(arg) > 1 && arg[0] == '-')
    {
      int is_token = 0;
      flags buffer = { 0, 0 };
      for (int b = 1; b < strlen(arg); b++)
      {
        char ltr = arg[b];
        if (ltr == 'n') buffer.no_newline = 1;
        else if (ltr == 'e') buffer.disable_interpret = 1;
        else {
          is_token = 1;
          buffer = (flags) {0,0};
          break;
        }
      }
      flag.no_newline |= buffer.no_newline;
      flag.disable_interpret |= buffer.disable_interpret;
      if (!is_token)
        continue;
    }

    // This argument is not an option/flag its a path to be cat'ed
    tokens[token_cnt++] = arg;
    parse_opt = 0;
  }

  for (int a = 0; a < token_cnt; a++)
    printf("%s", tokens[a]);

  if (!flag.no_newline)
    printf("\n");

  return 0;
}
