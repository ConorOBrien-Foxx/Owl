#include <stdlib.h>
#include "memory.h"

owl_memory memory_init(size_t cap) {
    owl_memory res;
    res.cap = cap;
    res.data = malloc(sizeof(*res.data) * res.cap);
    return res;
}

int memory_grow(owl_memory mem) {
    mem.cap *= 2;
    // no i don't care about type safety
    mem.data = realloc(mem.data, mem.cap * sizeof(*mem.data));
    // TODO: maybe care about it
    return 1;
}

atom memory_get(owl_memory mem, size_t index) {
    if(index >= mem.cap) {
        memory_grow(mem);
    }
    return mem.data[index];
}

void memory_set(owl_memory mem, size_t index, atom value) {
    if(index >= mem.cap) {
        memory_grow(mem);
    }
    mem.data[index] = value;
}