#ifndef OWL_STACK_INCLUDE
#define OWL_STACK_INCLUDE

#include "common.h"
#include "memory.h"

typedef struct owl_stack {
    owl_memory data;
    size_t ptr;
} owl_stack;

owl_stack   stack_init(size_t);
void        stack_push(owl_stack*, atom);
atom        stack_pop(owl_stack*);
void        stack_debug(owl_stack);

#endif