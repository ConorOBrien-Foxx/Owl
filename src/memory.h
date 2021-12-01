#ifndef OWL_MEMORY_INCLUDE
#define OWL_MEMORY_INCLUDE

#include "common.h"

typedef struct owl_memory {
    size_t cap;
    atom*  data;
} owl_memory;

owl_memory  memory_init(size_t);
int         memory_grow(owl_memory);
atom        memory_get(owl_memory, size_t);
void        memory_set(owl_memory, size_t, atom);

#endif