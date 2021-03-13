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

char* strcpy(char *dest, char *src)
{
    int i;

    for (i = 0; src[i] != '\0'; i++)
        dest[i] = src[i];

    return dest;
}

char* strncpy(char *dest, char *src, int n)
{
    int i;

    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for ( ; i < n; i++)
        dest[i] = '\0';

    return dest;
}
