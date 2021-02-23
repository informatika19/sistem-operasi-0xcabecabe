/**
 * lib.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 20 Februari 2021
 */

#include "lib.h"

int mod(int a, int b)
{
    int hasil = a/b;
    return a - b*hasil;
}
