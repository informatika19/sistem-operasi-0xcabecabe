/**
 * math.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 20 Februari 2020
 *
 * Header yang berisi fungsi-fungsi matematika
 */

#include "math.h"

int mod(int a, int b)
{
    int hasil = a/b;
    return a - b*hasil;
}

int div(int a, int b)
{
    return a/b;
}
