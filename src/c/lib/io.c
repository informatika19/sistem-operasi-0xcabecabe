/**
 * io.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 13 Maret 2021
 *
 * implementasi io.h
 */

#include "io.h"

#include "lib_asm.h"
#include "boolean.h"
#include "math.h"
#include "fileIO.h"

void print(char *str) {
    interrupt(0x21, 0x0000, str, 0, 0);
}

char *read(char *buf) {
    interrupt(0x21, 0x0001, buf, 0, 0);
    print("\n");
    return buf;
}

void printNumber(int number) {
    char c[2];

    if (number < 0) {
        print("-");
        number *= -1;
    }

    if (number < 10) {
        *c = number + '0';
        *(c + 1) = 0;
        print(c);
    } else {
        printNumber(div(number, 10));
        printNumber(mod(number, 10));
    }
}
