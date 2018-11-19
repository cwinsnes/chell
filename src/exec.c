#include "exec.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wait.h>

#define DELIMITERS " \t"

struct command* parse_command(char *input)
{
  char tmp[strlen(input)];
  strcpy(tmp, input);

  struct command *command = malloc(sizeof(struct command));

  char *token = strtok(tmp, DELIMITERS);
  if (token == NULL) {
    token = "";
  }
  command->exe = malloc(sizeof(char) * strlen(token)+1);
  strcpy(command->exe, token);

  size_t num_args = 1;
  size_t arg_buf = 4;
  command->args = malloc(sizeof(char *) * arg_buf);
  command->args[0] = command->exe;

  while ((token = strtok(NULL, DELIMITERS)) != NULL) {
    command->args[num_args] = malloc(sizeof(char) * strlen(token)+1);
    strcpy(command->args[num_args], token);
    num_args += 1;

    if (num_args == arg_buf) {
      arg_buf *= 2;
      command->args = realloc(command->args, arg_buf);
    }
  }
  command->args[num_args] = NULL;
  command->num_args = num_args;

  return command;
}

void free_command(struct command *command)
{
  int i;
  // Note: No need to free command->exe because command->args[0]
  // points to the same memory.
  for (i=0; i<command->num_args; ++i) {
    free(command->args[i]);
  }
  free(command->args);
  free(command);
}


void execute_command(struct command *command)
{
  if (!strcmp(command->exe, "")) {
    return;
  }

  pid_t pid = fork();
  if (pid == 0) {
    execvp(command->exe, command->args);
    perror(command->exe);
  }

  wait(NULL);
}
