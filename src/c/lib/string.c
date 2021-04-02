/**
 * string.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 6 Maret 2021
 *
 * Implementasi string.h
 */

#include "string.h"
#include "io.h"
#include "math.h"

#include "boolean.h"

int strlen(char *s) {
    int len = 0;
    while (s[len++] != '\0')
        ;

    return len - 1;
}

int strcmp(char *s1, char *s2) {
    for (; *s1 && *s1 == *s2; s1++, s2++)
        ;
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int strncmp(char *s1, char *s2, int n) {
    for (; *s1 && n && *s1 == *s2; n--, s1++, s2++)
        ;
    return (*(unsigned char *)s1 - *(unsigned char *)s2) * (n != 0);
}

char *strcpy(char *dest, char *src) {
    int i;

    for (i = 0; src[i] != '\0'; i++) dest[i] = src[i];
    dest[i] = 0;

    return dest;
}

char *strncpy(char *dest, char *src, int n) {
    int i;

    for (i = 0; i < n && src[i] != '\0'; i++) dest[i] = src[i];
    for (; i < n; i++) dest[i] = '\0';

    return dest;
}

char *strcat(char *dest, char *src) {
    int n_dest = strlen(dest);
    int i;

    for (i = 0; src[i] != '\0'; i++) {
        dest[n_dest + i] = src[i];
    }
    dest[n_dest + i] = '\0';
    return dest;
}

char *strncat(char *dest, char *src, int n) {
    int n_dest = strlen(dest);
    int i;

    for (i = 0; i < n && src[i] != '\0'; i++) {
        // disini '\0' dari dest keoverwrite sama src[0]
        dest[n_dest + i] = src[i];
    }
    dest[n_dest + i] = '\0';
    return dest;
}

int tokenize(char *src, char *dest, char delim) {
    int i, j;

    src = src + (1 * (*src == delim)); // remove delim at beginning
    j = strlen(src) - 1;
    *(src + j) = *(src + j) * (*(src + j) != delim); // remove delim at end

    i = 0, j = 0;
    while (*src != '\0') {
        if (*src == delim) {
            *(dest + j + i) = 0;
            j += 14 * (i != 0);
            i = 0;
        } else {
            *(dest + j + i) = *src;
            i++;
        }
        src++;
    }

    *(src + j + i) = 0;

    return div(j, 14) + 1;
}
