#include "exec.h"
#include "hashm.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wait.h>

#define DELIMITERS " \t"
#define PIPE "|"

void read_command_string(char *input, struct command *command)
{
  if (input == NULL) {
    input = "";
  }
  char tmp[strlen(input)+1];
  strcpy(tmp, input);

  char *savepointer = NULL;
  char *token = strtok_r(tmp, DELIMITERS, &savepointer);
  if (token == NULL) {
    token = "";
  }
  command->exe = malloc(sizeof(char) * strlen(token)+1);
  strcpy(command->exe, token);

  size_t num_args = 1;
  size_t arg_buf = 4;
  command->args = malloc(sizeof(char *) * arg_buf);
  command->args[0] = command->exe;

  while ((token = strtok_r(NULL, DELIMITERS, &savepointer)) != NULL) {
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
}

// TODO: Change all parsing into some kind of simple
//       recursive descent parser or similar.
//       Should make it easier than relying on strtok.
struct command* parse_command(char *input)
{
  struct command *first = NULL;
  struct command *previous = NULL;
  char cpy[strlen(input)+1];
  strcpy(cpy, input);
  char *savepointer = NULL;
  char *currcmd = strtok_r(cpy, PIPE, &savepointer);
  do {
    struct command *command = malloc(sizeof(struct command));
    read_command_string(currcmd, command);

    if (first == NULL) {
      first = command;
      first -> pipeto = NULL;
      previous = first;
    } else {
      previous -> pipeto = command;
      command -> pipeto = NULL;
      previous = command;
    }
    currcmd = strtok_r(NULL, PIPE, &savepointer);
  } while(currcmd != NULL);

  return first;
}

void free_command(struct command *command)
{
  size_t i;
  // Note: No need to free command->exe because command->args[0]
  // points to the same memory.
  for (i=0; i<command->num_args; ++i) {
    free(command->args[i]);
  }
  free(command->args);
  if ((command -> pipeto) != NULL) {
    free_command(command -> pipeto);
  }
  free(command);
}

void execute_command(struct command *command, struct hashmap *builtins)
{
  if (!strcmp(command->exe, "")) {
    return;
  }

  if (hashm_haskey(builtins, command->exe)) {
    void (*func)(struct command*);
    hashm_get(builtins, command->exe, (void**)&func);
    func(command);
    return;
  }

  int status;
  pid_t pid;
  int save_in = dup(STDIN_FILENO);
  do {
    int fd[2];
    if (command -> pipeto) {
      pipe(fd);
    }
    pid = fork();
    if (pid == 0) {
      if (command -> pipeto) {
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
      }
      execvp(command->exe, command->args);
      perror(command->exe);
    }
    if (command -> pipeto) {
      dup2(fd[0], STDIN_FILENO);
      close(fd[0]);
      close(fd[1]);
    }
    command = command -> pipeto;
  } while(command != NULL);

  while(waitpid(-getpgid(0), &status, 0) > 0) {} // Wait for all children
  dup2(save_in, STDIN_FILENO);
}
