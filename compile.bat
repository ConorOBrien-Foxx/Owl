@ECHO OFF
REM -Wno-format because gcc complains about %lu for size_t, and
REM my compiler doesn't support %zu
gcc -g -Wall -Wno-format src/owl.c src/memory.c src/stack.c src/interpreter.c -o owl