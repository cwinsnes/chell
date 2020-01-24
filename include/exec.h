#ifndef CHELL_EXEC_H
#define CHELL_EXEC_H

#include <stdlib.h>
#include "hashm.h"

struct command {
  char *exe;
  char **args;
  size_t num_args;
  struct command *pipeto;
};

/**
 * Parse a command from an input string.
 * A command is defined as sets characters separated by whitespace.
 *
 * @param input String to be parsed.
 @ @result A command parsed from the input string.
 */
struct command* parse_command(char *);

/**
 * Deallocates all memory allocated by a command.
 *
 * @param command Command to be deallocated.
 *
 * Notes:
 * The command should not be accessed after it has been deallocated.
 */
void free_command(struct command*);

/**
 * Fork and execute a command.
 * Will not execute anything if the command is equal to the empty
 * string.
 * @param command Command to be executed.
 * @param builtins A hashmap containing all builtin commands.
 */
void execute_command(struct command*, struct hashmap*);

#endif
