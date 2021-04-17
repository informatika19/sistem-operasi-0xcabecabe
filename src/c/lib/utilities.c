/**
 * utilities.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 18 April 2021
 *
 * implementasi utilities.h
 */

#include "utilities.h"

#include "lib_asm.h"

char *fillBuffer(char *buf, int n, char filler) {
    int i;
    for (i = 0; i < n; ++i) {
        *(buf + i) = filler;
    }

    return buf;
}

void exec(char *fileName, int segment, int *success, char parentIndex) {
    interrupt(0x21, parentIndex * 0x100 + 0x06, fileName, segment, success);
}
