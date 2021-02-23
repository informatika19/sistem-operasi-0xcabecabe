/**
 * lib.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 20 Februari 2020
 */

#include "lib.h"

int mod(int a, int b)
{
    int hasil = a/b;
    return a - b*hasil;
}
