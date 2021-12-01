#ifndef OWL_COMMON_INCLUDE
#define OWL_COMMON_INCLUDE

#include <stdio.h>
extern int DEBUG;
#define debug(...) if(DEBUG) printf(__VA_ARGS__)

typedef size_t atom;

#endif