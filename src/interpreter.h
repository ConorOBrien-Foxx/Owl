#ifndef OWL_INTERPRETER_INCLUDE
#define OWL_INTERPRETER_INCLUDE

#include "common.h"
#include "memory.h"
#include "stack.h"

#define MEM_OPFLAG  (6)
#define PC_START    (100)

typedef enum owl_state {
    READING,
    COMMAND,
    REGISTER,
    READ_NEXT,
} owl_state;

typedef struct owl_interpreter {
    owl_state   state;
    char        build;
    char        aux;
    char        regbuild;
    atom        acc;
    size_t      ycount;
    size_t      mulcount;
    int         dreg; // desired register count
    int         dpref; // desired prefix length
    atom        operands[3];
    int         opcount;
    owl_memory  mem;
    size_t      pc;
    int         in_comment;
    /*loop info*/
    owl_stack   loopstack;
    size_t      depth;
    int         skipping;
} owl_interpreter;

atom            owl_memget(owl_interpreter*, size_t);
void            owl_memset(owl_interpreter*, size_t, atom);
void            owl_punctuate(owl_interpreter*);
void            owl_feed(owl_interpreter*, unsigned char);
owl_interpreter owl_init();
void            owl_debug(owl_interpreter);
void            owl_run(owl_interpreter*);

#endif