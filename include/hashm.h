#ifndef CHELL_HASHM_H
#define CHELL_HASHM_H

#include <stdlib.h>

struct hash_node {
  char *key;
  int used;
  void * value;
  struct hash_node *next;
};

struct hashmap {
  size_t size;
  size_t capacity;
  struct hash_node *entries;
};

/**
 * Creates, intiializes, and returns a hashmap.
 * The returned map should only be deleted using `hashm_destroy`.
 * @arg initsize How large space to allocate initially.
 */
struct hashmap* hashm_create(size_t);
/**
 * Destroys an initialized hashmap and all memory is freed.
 * After this operation, the hashmap should not be used again.
 *
 * @arg map The hashmap to be destroyed.
 */
void hashm_destroy(struct hashmap*);

/**
 * Hashes a string into an integer.
 * @arg key The string to be hashed.
 */
unsigned long hashm_hash(const char *);

/**
 * Inserts a value into a hashmap.
 * @arg map The map to have a value inserted.
 * @arg key The key to be used for the key-value pair.
 * @arg value The value to be inserted.
 */
void hashm_insert(struct hashmap*, char*, void *);

/**
 * Retrieves a value from a hashmap.
 * @arg map The map to retrieve a value from.
 * @arg key The key to be used for retrieval.
 * @arg value A pointer to a void * in which to
 *            store the retrieved value.
 */
void hashm_get(struct hashmap*, char*, void **);

/**
 * Checks for the existance of a key in the hashmap.
 * Returns 0 if no key was found, otherwise a positive integer.
 * @arg map The map to check.
 * @arg The key to look for.
 */
int hashm_haskey(struct hashmap*, char*);

#endif
