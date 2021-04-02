/**
 * folderIO.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 2 April 2021
 *
 * Implementasi folderIO.h
 */

#include "folderIO.h"

#include "../kernel.h"
#include "fileIO.h"
#include "math.h"
#include "teks.h"

int getChildrenFiles(char *path, int parentIndex, char *res) {
    int i = 0, j = 0;
    int test;
    char dir[2 * SECTOR_SIZE];
    char tmpName[15]; // 15 soalnya buat '/' di akhir

    test = getFileIndex(path, parentIndex);

    if (test == -1) {
        return -2;
    }

    parentIndex = test & 0xFF;

    readSector(dir, 0x101);
    readSector(dir + SECTOR_SIZE, 0x102);

    if (parentIndex != '\xFF' && *(dir + (parentIndex * 0x10) + 1) != '\xFF') {
        return -1;
    }

    while (i < 1024) {
        if (*(dir + i) == parentIndex && *(dir + i + 2) != 0) {
            clear(tmpName, 15);
            strncpy(tmpName, dir + i + 2, 14);
            if (*(dir + i + 1) == '\xFF') strncat(tmpName, "/", 1);
            strncpy(res + j, tmpName, 15);
            j += 15;
        }
        i += 16;
    }

    return div(j, 15);
}
