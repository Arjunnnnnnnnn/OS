#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define CURRENT_DIR "./"

typedef struct entry
{
  char *name;
  long node_i;
} entry;

typedef struct flags
{
  
  int b;
  int node_i;
  int l;
} 
flags;

int ERR_STATUS = 0;

void display_entry(entry entry, flags flag)
{
  if (flag.l)
    if (flag.node_i)
      printf("%ld %s\n ", entry.node_i, entry.name);
    else
      printf("%s\n", entry.name);
  else
    if (flag.node_i)
      printf("%ld %s ", entry.node_i, entry.name);
    else
      printf("%s ", entry.name);
}

int get_entry_cnt(char *path)
{
  int cnt = 0;

  DIR *active_dir = opendir(path);
  struct dirent *entry;
  while ((entry = readdir(active_dir)) != NULL)
    cnt++;

  closedir(active_dir);
  return cnt;
}

int check_file_exists_is_file(char *path, flags flag)
{
  struct stat f_status = { 0 };

  if(stat(path, &f_status) == -1)
  {
    fprintf(stderr, "ls: cannot access '%s': No such file or directory\n", path);
    ERR_STATUS = 1;
    return 1;
  }

  if (S_ISREG(f_status.st_mode))
  {
    if (!flag.l) printf("\n");
    display_entry((entry) { path, f_status.st_ino }, flag);
    return 1;
  }

  return 0;
}

int compare(const void *a, const void *b)
{
  return strcmp(((entry *) a)->name, ((entry *) b)->name);
}

void ls_directory(char *path, flags flag)
{
  DIR *active_dir = opendir(path);

  int entry_cnt = get_entry_cnt(path), active_entry_cnt = 0;
  entry curr_dir, parent_dir;
  entry entries[entry_cnt];
  struct dirent *raw_entry;

  while((raw_entry = readdir(active_dir)))
  {
    char *name = raw_entry->d_name;
    long node_i = raw_entry->d_ino;

    entry refined_entry = { name, node_i };

    if (strcmp(name, ".") == 0)
    {
      curr_dir = refined_entry;
      continue;
    }

    if (strcmp(name, "..") == 0)
    {
      parent_dir = refined_entry;
      continue;
    }

    if (!flag.b && name[0] == '.')
      continue;

    entries[active_entry_cnt++] = refined_entry;
  }

  closedir(active_dir);

  if (flag.b)
  {
    // First print the dot base dirs
    display_entry(curr_dir, flag);
    display_entry(parent_dir, flag);
  }

  if (active_entry_cnt > 0)
  {
    // Sort the dir and file name arrays
    qsort(entries, active_entry_cnt, sizeof(entries[0]), compare);

    // Then print rest of the entries
    for (int i = 0; i < active_entry_cnt; i++)
      display_entry(entries[i], flag);
  }
}

int main(int ar, char *argv[])
{
  char *paths[ar];
  int path_cnt = 0;
  flags flag = { 0, 0, 0 };

  for (int i = 1; i < ar; i++)
  {
    char *arg = argv[i];

    // Double dash keyword options
    if (strlen(arg) > 2 && arg[0] == '-' && arg[1] == '-')
    {
      if (strcmp(arg, "--b") == 0) flag.b = 1;
      else if (strcmp(arg, "--node_i") == 0) flag.node_i = 1;
      else
      {
        fprintf(stderr, "ls: unrecognized option '%s'\n", arg);
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
        if (letter == 'a') flag.b = 1;
        else if (letter == 'i') flag.node_i = 1;
        else if (letter == '1') flag.l = 1;
        else
        {
          fprintf(stderr, "ls: invalid option -- '%c'\n", letter);
          exit(EXIT_FAILURE);
        }
      }
      continue;
    }

    // This argument is not an option/flag its a path to be ls'ed
    paths[path_cnt++] = arg;
  }

  // If no path is provided in the args, we take the cwd
  if (path_cnt == 0)
  {
    paths[0] = CURRENT_DIR;
    path_cnt++;
  }

  if (path_cnt == 1)
  {
    char *path = paths[0];
    if (!check_file_exists_is_file(path, flag))
      ls_directory(path, flag);
  }
  else
  {
    for (int i = 0; i < path_cnt; i++)
    {
      char *path = paths[i];

      if (check_file_exists_is_file(path, flag))
      {
        printf("\n");
        continue;
      }

      if (i > 0 && !flag.l)
        printf("\n");
      printf("%s:\n", path);
      ls_directory(path, flag);
      printf("\n");
    }
  }

  return ERR_STATUS;
}
