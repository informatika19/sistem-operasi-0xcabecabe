/**
 * string.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 6 Maret 2021
 */

#include "string.h"

int strlen(char *s)
{
    int len = 0;
    while (s[len++] != '\0');

    return len-1;
}
