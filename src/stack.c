#include <stdio.h>
#include "stack.h"

owl_stack stack_init(size_t cap) {
    owl_stack res = { memory_init(cap), 0 };
    return res;
}

void stack_push(owl_stack* stack, atom value) {
    memory_set(stack->data, stack->ptr, value);
    stack->ptr++;
}

atom stack_pop(owl_stack* stack) {
    if(stack->ptr) {
        return memory_get(stack->data, --stack->ptr);
    }
    //TODO: stderr
    puts("Popping from an empty stack.");
    return -1;
}

void stack_debug(owl_stack stack) {
    printf("<stack>\n| Stack ptr: %lu\n", stack.ptr);
    for(size_t i = 0; i < stack.ptr; i++) {
        printf("| %lu: %lu\n", i, stack.data.data[i]);
    }
}