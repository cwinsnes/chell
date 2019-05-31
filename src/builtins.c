#include "builtins.h"
#include "hashm.h"
#include "errno.h"
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "globals.h"

#define CD "cd"
#define EXIT "exit"

void changedir(struct command *cmd)
{
  if (cmd -> num_args > 2) {
    fprintf(stderr, "%s: Too many arguments\n", cmd->exe);
    return;
  }
  int ret = chdir(cmd->args[1]);
  if (ret) {
    perror(cmd->exe);
  }
}

void chell_exit(struct command *cmd)
{
  int exit_code = 0;
  if (cmd -> num_args > 1) {
    // atoi returns 0 on invalid number
    exit_code = abs(atoi(cmd -> args[1]));
  }

  CHELL_RUNNING = -exit_code;
}

struct hashmap* initialize_builtins(void)
{
  struct hashmap *builtins = hashm_create(32);
  hashm_insert(builtins, CD, changedir);
  hashm_insert(builtins, EXIT, chell_exit);
  return builtins;
}

void destroy_builtins(struct hashmap *builtins)
{
}
