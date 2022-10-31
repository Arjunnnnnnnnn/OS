#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

//Starts here
typedef struct flags
{
  int utc;
  int rfc_email;
}
 flags;

int main(int ar, char *argv[])
{
  int opr_cnt =0;
  char *date_format_string;

  flags flag = {0,0};

  for (int a = 1; a < ar; a++)
  {
    char *arg = argv[a];

    // Double dash keyword options
    if (strlen(arg) > 2 && arg[0] == '-' && arg[1] == '-')
    {
      if (strcmp(arg, "--universal") == 0) flag.utc = 1;
      else if (strcmp(arg, "--utc") == 0) flag.utc = 1;
      else if (strcmp(arg, "--rfc-email") == 0) flag.rfc_email = 1;
      else
      {
        fprintf(stderr, "date: unrecognized option '%s'\n", arg);
        exit(EXIT_FAILURE);
      }
      continue;
    }

    // Single dash keyword ltr options
    if (strlen(arg) > 1 && arg[0] == '-')
    {
      for (int j = 1; j < strlen(arg); j++)
      {
        char let = arg[j];
        if (ltr == 'u') flag.utc = 1;
        else if (ltr == 'R') flag.rfc_email = 1;
        else
        {
          fprintf(stderr, "date: invalid option -- '%c'\n", ltr);
          exit(EXIT_FAILURE);
        }
      }
      continue;
    }

    if (opr_cnt > 0)
    {
      fprintf(stderr, "date: extra operand '%s'\n", arg);
      exit(EXIT_FAILURE);
    }

    if (arg[0] != '+')
    {
      fprintf(stderr, "date: invalid date '%s'\n", arg);
      exit(EXIT_FAILURE);
    }

    date_format_string = &arg[1];
    opr_cnt++;
  }

  if (opr_cnt == 1 && flag.rfc_email)
  {
    fprintf(stderr, "date: multiple output formats specified\n");
    exit(EXIT_FAILURE);
  }

  time_t time_current = time(NULL);
  struct tm time_s = flag.utc ?
    *gmtime(&time_current) :
    *localtime(&time_current);

  if (opr_cnt == 0)
    date_format_string = "%a %d %b %T %Z %Y";

  if (flag.rfc_email)
    date_format_string = "%a, %d %b %Y %T %z";

  char *formatted_date = (char *) calloc(100, sizeof(char));
  strftime(formatted_date, 100, date_format_string, &time_s);
  printf("%s\n", formatted_date);

  fr(formatted_date);
  return 0;
}
