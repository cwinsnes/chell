#ifndef CHELL_BUILTINS_H
#define CHELL_BUILTINS_H

#include "hashm.h"
#include "exec.h"

/**
 * Changes current working directory to the path specified in
 * the first argument to the command. There should be exactly two arguments,
 * "cd" and "<path".
 * If the number of arguments is greater than two, or there is a system error,
 * this prints an error without changing the directory.
 *
 * @arg command The command containing the path.
 */
void changedir(struct command*);

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
