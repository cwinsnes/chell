#include "builtins.h"
#include "hashm.h"
#include "errno.h"
#include <stdio.h>
#include <unistd.h>

#define CD "cd"

void changedir(struct command *cmd)
{
  if (cmd -> num_args > 2) {
    fprintf(stderr, "%s: Too many arguments\n", cmd->exe);
  }
  int ret = chdir(cmd->args[1]);
  if (ret) {
    perror(cmd->exe);
  }
}

struct hashmap* initialize_builtins(void)
{
  struct hashmap *builtins = hashm_create(32);
  hashm_insert(builtins, "cd", changedir);
  return builtins;
}

void destroy_builtins(struct hashmap *builtins)
{
}
