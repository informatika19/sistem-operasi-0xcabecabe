/**
 * utilities.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 18 April 2021
 *
 * implementasi utilities.h
 */

#include "utilities.h"

#include "boolean.h"
#include "fileIO.h"
#include "lib_asm.h"
#include "string.h"

char *fillBuffer(char *buf, int n, char filler) {
    int i;
    for (i = 0; i < n; ++i) {
        *(buf + i) = filler;
    }

    return buf;
}

void exec(char *fileName, int segment, int *success, char parentIndex) {
    interrupt(0x21, parentIndex * 0x100 + 0x06, fileName, segment, success);
    // if (*success) {
    //     while (true)
    //         ;
    // }

    return;
}

int sendArguments(char *args, char cwdIndex) {
    char argvTmp[512]; // MAXIMUM_CMD_LEN == 20
    char cwdIdxStr[3];
    int secSize = 1;

    // removes argv.tmp in case it exists
    removeFile("argv.tmp", 0, 0xFF);

    fillBuffer(argvTmp, 512, 0);
    fillBuffer(cwdIdxStr, 3, 0);

    itoa(cwdIdxStr, (cwdIndex << 2) >> 2, 3);
    strncpy(argvTmp, cwdIdxStr, 3);
    strncat(argvTmp, " ", 1);
    strcat(argvTmp, args);

    updateFile(argvTmp, "argv.tmp", &secSize, 0xFF);

    return secSize < 0 ? -1 : 0;
}

int getArguments(char *argv) {
    char cmd[512];
    int res, i;

    getFile(cmd, "argv.tmp", &res, 0xFF);
    if (res < 0) {
        return -1;
    }

    // remove argv.tmp
    removeFile("argv.tmp", 0, 0xFF);

    res = strntoken(cmd, argv, ' ', 20);
    return res;
}
