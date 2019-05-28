#include "hashm.h"
#include <stdlib.h>
#include <string.h>

struct hashmap* hashm_create(size_t initsize)
{
  struct hashmap *map = malloc(sizeof(struct hashmap));
  if (!map) {
    return NULL;
  }
  map -> size = 0;
  map -> capacity = initsize;
  map -> entries = calloc(initsize, sizeof(struct hash_node));
  if (!map -> entries) {
    free(map);
    return NULL;
  }
  return map;
}

unsigned long hashm_hash(const char *key)
{
  //djb2 algorithm
  unsigned long hash = 5381;
  char c;
  while((c = *key++)) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash;
}

void hashm_insert(struct hashmap *map, char *key, void *value)
{
  unsigned long hash = hashm_hash(key);
  size_t index = hash % map -> capacity;

  struct hash_node *list = &(map -> entries[index]);
  while (list->used) {
    list = list -> next;
  }
  list -> key = malloc(strlen(key)+1);
  strcpy(list -> key, key);
  list -> value = value;
  list -> used = 1;
  list -> next = calloc(1, sizeof(struct hash_node));
  map -> size += 1;
}

void hashm_get(struct hashmap *map, char *key, void **value)
{
  unsigned long hash = hashm_hash(key);
  size_t index = hash % map -> capacity;
  struct hash_node list = map -> entries[index];
  while (list.used && strcmp(list.key, key)) {
    list = *list.next;
  }

  // If list.used is false, value should be NULL
  *value = list.value;
}

int hashm_haskey(struct hashmap *map, char *key)
{
  unsigned long hash = hashm_hash(key);
  size_t index = hash % map -> capacity;
  struct hash_node list = map -> entries[index];
  while (list.used && strcmp(list.key, key)) {
    list = *list.next;
  }
  return list.used;
}
