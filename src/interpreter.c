#include "interpreter.h"
#include "common.h"

int vowel_index(char c) {
    switch(c) {
        case 'a': return 0;
        case 'e': return 1;
        case 'i': return 2;
        case 'o': return 3;
        case 'u': return 4;
        case 'y': return 5;
        default:  return -1;
    }
}

atom owl_memget(owl_interpreter* n, size_t index) {
    return memory_get(n->mem, index);
}
void owl_memset(owl_interpreter* n, size_t index, atom value) {
    memory_set(n->mem, index, value);
}

// only punctuate if actually executing instructions
void owl_punctuate(owl_interpreter* n) {
    atom dat;
    switch(n->build) {
        case 'a':
            debug(">>> $%lu = %lu;\n", n->operands[0], n->operands[1]);
            // res.mem[n->operands[0]] = n->operands[1];
            owl_memset(n, n->operands[0], n->operands[1]);
            break;
        case 'e':
            dat = n->mem.data[n->operands[0]];
            switch(n->mem.data[MEM_OPFLAG]) {
                case 0:
                    debug(">>> putchar($%lu);\n", n->operands[0]);
                    putchar(dat);
                    break;
                case 1:
                    debug(">>> println($%lu);\n", n->operands[0]);
                    printf("%lu\n", dat);
                    break;
                case 2:
                    debug(">>> getchar($%lu);\n", n->operands[0]);
                    owl_memset(n, n->operands[0], getchar());
                    break;
                case 3:
                    debug(">>> print($%lu);\n", n->operands[0]);
                    printf("%lu", dat);
                    break;
            }
            debug("\n"); // empty line after output
            break;
        case 'i':
            dat = n->mem.data[n->operands[0]];
            debug(">>> loop iter (with $%lu = %lu)\n", n->operands[0], dat);
            n->skipping = n->depth = !dat;
            if(!n->skipping) {
                debug("Pushing address: %lu (0x%x)\n", n->pc, n->pc);
                // the instruction before
                stack_push(&n->loopstack, n->pc - 1);
                // stack_debug(n->loopstack);
            }
            break;
        case 'o':
            debug("Sub property: %c\n", n->aux);
            switch(n->aux) {
                case 'a':
                    debug("$%lu = $%lu + $%lu\n", n->operands[0], n->operands[1], n->operands[2]);
                    owl_memset(n, n->operands[0],
                        owl_memget(n, n->operands[1]) + owl_memget(n, n->operands[2])
                    );
                    break;
                case 'e':
                    debug("$%lu = $%lu - $%lu\n", n->operands[0], n->operands[1], n->operands[2]);
                    owl_memset(n, n->operands[0],
                        owl_memget(n, n->operands[1]) - owl_memget(n, n->operands[2])
                    );
                    break;
                case 'i':
                    debug("$%lu = $%lu * $%lu\n", n->operands[0], n->operands[1], n->operands[2]);
                    owl_memset(n, n->operands[0],
                        owl_memget(n, n->operands[1]) * owl_memget(n, n->operands[2])
                    );
                    break;
                case 'o':
                    debug("$%lu = $%lu / $%lu\n", n->operands[0], n->operands[1], n->operands[2]);
                    owl_memset(n, n->operands[0],
                        owl_memget(n, n->operands[1]) / owl_memget(n, n->operands[2])
                    );
                    break;
                case 'u':
                    debug("$%lu = $%lu %% $%lu\n", n->operands[0], n->operands[1], n->operands[2]);
                    owl_memset(n, n->operands[0],
                        owl_memget(n, n->operands[1]) % owl_memget(n, n->operands[2])
                    );
                    break;
                case 'y':
                    debug("$%lu = mem[$%lu + $%lu]\n", n->operands[0], n->operands[1], n->operands[2]);
                    size_t addr = owl_memget(n, n->operands[2])+ owl_memget(n, n->operands[1]);
                    debug("-> %lu\n", addr);
                    owl_memset(n, n->operands[0], owl_memget(n, addr));
                    break;
                default:
                    printf("Unhandled subinstruction: o%c\n", n->aux);
                    break;
            }
            break;
        default:
            fprintf(stderr, "Unhandled instruction: '%c'\n", n->build);
            break;
    }
}

#define COMMENT_SYMBOL ('|')
void owl_feed(owl_interpreter* n, unsigned char c) {
    if(n->state == READING) {
        if(c == COMMENT_SYMBOL) {
            n->in_comment = 1;
        }
        else if(n->in_comment && c == '\n') {
            n->in_comment = 0;
        }
        if(n->in_comment) {
            return;
        }
    }
    
    int vi = vowel_index(c);
    if(vi < 0) {
        return;
    }
    
    if(!n->skipping) {
        debug(" 0x%-3x | '%c'\n", n->pc, c);
    }
    
    switch(n->state) {
        case READING:
            owl_memset(n, n->pc++, c);
            break;
        case REGISTER:
            if(n->dpref < 0) {
                n->regbuild = c;
                n->dpref = 0;
                switch(c) {
                    case 'i':
                        n->acc++;
                    case 'e':
                        n->acc++;
                    case 'a':
                        break;
                    case 'y':
                        n->dpref++;
                    case 'u':
                        n->dpref++;
                    case 'o':
                        n->dpref++;
                        break;
                }
            }
            else {
                n->acc *= 6;
                n->acc += vi;
                if(c == 'y') n->ycount++;
                n->dpref--;
            }
            if(!n->dpref) {
                switch(n->regbuild) {
                    case 'a':
                    case 'e':
                    case 'i':
                        break;
                    case 'o':
                        n->acc += 3;
                        break;
                    case 'u':
                        n->acc += 9;
                        break;
                    case 'y':
                        n->acc += 45;
                        break;
                }
                if(n->ycount == 3) {
                    n->acc = 0;
                    n->dpref = -1;
                    n->ycount = 0;
                    n->mulcount++;
                }
                else {
                    n->operands[n->opcount++] = n->acc + 260 * n->mulcount;
                    n->acc = 0;
                    n->dpref = -1;
                    n->ycount = 0;
                    n->mulcount = 0;
                    debug("Operand #%lu added: $%lu\n", n->opcount, n->operands[n->opcount - 1]);
                    if(!--n->dreg) {
                        n->state = COMMAND;
                        if(!n->skipping) {
                            owl_punctuate(n);
                        }
                        n->opcount = 0;
                    }
                }
            }
            break;
        case COMMAND:
            n->build = c;
            debug("Command: '%c'\n", c);
            switch(c) {
                case 'a':
                    n->dreg = 2;
                    n->state = REGISTER;
                    break;
                case 'e':
                    n->dreg = 1;
                    n->state = REGISTER;
                    break;
                case 'o':
                    n->dreg = 3;
                    n->state = READ_NEXT;
                    break;
                case 'i':
                    if(n->skipping) n->depth++;
                    n->dreg = 1;
                    n->state = REGISTER;
                    break;
                case 'u':
                    if(n->skipping) n->depth--;
                    if(!n->depth) {
                        if(n->skipping) {
                            n->skipping = 0;
                        }
                        else {
                            n->pc = stack_pop(&n->loopstack) - 1;
                        }
                    }
                    break;
            }
            break;
        case READ_NEXT:
            n->aux = c;
            n->state = REGISTER;
            break;
    }
}

#define INITIAL_MEM_SIZE    (1024)
#define INITIAL_STACK_SIZE  (8)
owl_interpreter owl_init() {
    owl_interpreter res;
    res.dreg = 0;
    res.dpref = -1;
    res.acc = 0;
    res.state = READING;
    res.opcount = 0;
    res.ycount = 0;
    res.mulcount = 0;
    res.mem = memory_init(INITIAL_MEM_SIZE);
    res.mem.data[MEM_OPFLAG] = 0; // oo: mode flag
    res.pc = PC_START;
    res.in_comment = 0;
    
    // res.loopstack = memory_init(INITIAL_STACK_SIZE);
    // res.loopptr = 0;
    res.loopstack = stack_init(INITIAL_STACK_SIZE);
    res.depth = 0;
    res.skipping = 0;
    return res;
}

void owl_debug(owl_interpreter n) {
    printf("state: %i\nacc: %lu\ndreg: %i\ndpref: %i\n", n.state, n.acc, n.dreg, n.dpref);
    printf("ops: [ ");
    for(size_t i = 0; i < n.opcount; i++) {
        printf("%i ", n.operands[i]);
    }
    puts("]\n");
}

void owl_run(owl_interpreter* n) {
    char c;
    while((c = n->mem.data[n->pc])) {
        owl_feed(n, c);
        n->pc++;
    }
}