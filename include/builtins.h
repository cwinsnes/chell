#ifndef CHELL_BUILTINS_H
#define CHELL_BUILTINS_H

#include "hashm.h"
#include "exec.h"

/**
 * Changes current working directory to the path specified in
 * the first argument to the command. There should be exactly
 * two arguments, "cd" and "<path".
 * If the number of arguments is greater than two, or there is a system error,
 * this prints an error without changing the directory.
 *
 * @arg command The command containing the path.
 */
void changedir(struct command*);

/**
 * Exits the current running instance of chell with an exit code.
 * @arg command A command struct, optionally containing an exit
                code to be used for exiting. There should be 2 arguments,
                "exit" and the optional exit code. If there are more than
                two arguments, only the second argument is considered
                (args[1]). If that argument is not present, or is not a
                number, chell exits with an exit code of 0.
 */
void chell_exit(struct command*);

/**
 * Creates and initializes a hashmap with key-value pairs
 * consisting of builtin-functions -> the corresponding
 * function-pointer.
 * The returned hashmap should be destroyed using `detroy_builtins`.
 */
struct hashmap* initialize_builtins(void);
/**
 * Destroys a hashmap containing builtin functions.
 * The map should have been created by `initialize_builtins`.
 *
 * @arg builtins The map to be destroyed.
 */
void destroy_builtins(struct hashmap*);

#endif
