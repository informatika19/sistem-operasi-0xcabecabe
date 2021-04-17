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

int strntoken(char *src, char *dest, char delim, int n) {
    int i, j;
    bool stop = false;

    for (; *src == delim; src++) // remove delim at beginning
        ;
    j = strlen(src) - 1;
    *(src + j) = *(src + j) * (*(src + j) != delim); // remove delim at end
    i = 0, j = 0;
    while (*src != '\0'  && !stop) {
        stop = i >= n;
        if (*src == delim || stop) {
            *(dest + j + i) = 0;
            j += n * (i != 0);
            i = 0;
        } else if (*src == '\\') {
            src++;
            *(dest + j + i) = *src;
            stop = *src == 0; // \ di akhir string
            i += 1 * !stop;
        } else {
            *(dest + j + i) = *src;
            i++;
        }
        src++;
    }

    *(dest + j + i) = 0;

    return div(j, n) + 1;
}

int atoi(char *str){
    int result = 0;
    char *c;
    for(c = str; *c != '\0' && *c >= '0' && *c <= '9'; ++c) {
        result= (*c - '0') + result*10;
    }
    return result;
}

char *itoa(char *buf, int i, int n) {
    char c;

    if(n > 0) {
        if (i < 0) {
            strncat(buf, "-", 1);
            i *= -1;
        }

        if (i < 10) {
            c = i + '0';
            strncat(buf, &c, 1);
        } else {
            itoa(buf, div(i, 10), n - 1);
            itoa(buf, mod(i, 10), n - 1);
        }

    }

    return buf;
}