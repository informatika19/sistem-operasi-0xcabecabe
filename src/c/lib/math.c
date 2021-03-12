/**
 * math.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 20 Februari 2020
 *
 * Header yang berisi fungsi-fungsi matematika
 */

#include "math.h"

int div(int a, int b)
{
    // return a/b + ((a < 0) ^ (b < 0));
    return a/b;
}

int mod(int a, int b)
{
    int hasil = div(a,b);
    return a - b*hasil;
}