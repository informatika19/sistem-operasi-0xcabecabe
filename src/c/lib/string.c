/**
 * string.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 6 Maret 2021
 */

#include "string.h"
#include "boolean.h"

int strlen(char *s)
{
    int len = 0;
    while (s[len++] != '\0');

    return len-1;
}

int strcmp(char *s1, char *s2)
{
    while (*s1 && *s1 == *s2)
    {
        ++s1;
        ++s2;
    }

    return *(unsigned char*)s1 - *(unsigned char*)s2;
}
