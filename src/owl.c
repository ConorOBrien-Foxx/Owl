#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"

int DEBUG = 0;

int main (int argc, char** argv) {
    if(argc < 2) {
        printf("Expected file name as input\n");
        return 1;
    }
    
    int arg;
    char* filename;
    for(arg = 1; arg < argc; arg++) {
        filename = argv[arg];
        if(filename[0] == '-' || filename[0] == '/') {
            switch(filename[1]) {
                case 'd':
                case 'D':
                    DEBUG = 1;
                    break;
                default:
                    fprintf(stderr, "Unrecognized command line option: %s\n", filename);
                    break;
            }
        }
        else {
            break;
        }
    }
    
    debug("!! Debug mode engaged !!\n");
    
    owl_interpreter n = owl_init();
    
    FILE* prog = fopen(filename, "r");
    char c;
    while(EOF != (c = fgetc(prog))) {
        owl_feed(&n, c);
    }
    
    debug("!! Done reading, initiating command mode !!\n");
    
    n.state = COMMAND;
    n.mem.data[n.pc] = 0; // terminating 0
    n.pc = PC_START;
    owl_run(&n);
}