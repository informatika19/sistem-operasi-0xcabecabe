/**
 * fileIO.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 13 Maret 2021
 *
 * implementasi fileIO.h
 */

#include "fileIO.h"

#include "io.h"
#include "lib_asm.h"
#include "math.h"
#include "teks.h"

char *getSector(char *buf, int secNo) {
    interrupt(0x21, 0x0002, buf, secNo, 0);
    return buf;
}

void updateSector(char *buf, int secNo) {
    interrupt(0x21, 0x0003, buf, secNo, 0);
}

char *getFile(char *buffer, char *path, int *result, char parentIndex) {
    interrupt(0x21, parentIndex * 0x100 + 0x04, buffer, path, result);
    return buffer;
}

int updateFile(char *buffer, char *path, int *sectors, char parentIndex) {
    interrupt(0x21, parentIndex * 0x100 + 0x05, buffer, path, sectors);
    return *sectors;
}

int getFileIndex(char *path, char parentIndex, char *dir) {
    char *entry;
    char tmpP[64][14];
    char parents[64][14];
    bool found;
    int tmp = 512 * 2;
    int i, j, jmlParents;

    if (*path == 0) {
        return parentIndex;
    }

    jmlParents = strntoken(path, tmpP, '/', 14);

    // Untuk handle . (dihapus)
    i = 0, j = 0;
    for (; i < jmlParents; i++) {
        if (strncmp(tmpP[i], ".", 2) == 0) {
            // do nothing
        } else {
            strcpy(parents[j], tmpP[i]);
            j++;
        }
    }

    jmlParents = j;
    j = 0;
    found = true;
    while (j < jmlParents && found) {
        found = false;
        // iterasi dalam file buat nyari yang parentIndexnya sesuai
        //      kalo ketemu, cari yang namanya sama strncmp(entry+2,...)
        //          kalo namanya ga sama, found = true
        if (strncmp(parents[j], "..", 14) == 0) {
            found = true;  // kasus .. sebagai elemen terakhir di parents
            if (parentIndex != '\xFF') {
                parentIndex = *(dir + (parentIndex * 0x10)) & 0xFF;
            }
            // kalo di root do nothing
        } else {
            i = 0;
            while (!found && i < tmp) {
                entry = dir + i;
                found = *entry == parentIndex
                            ? strncmp(entry + 2, parents[j], 14) == 0
                            : found;
                i += 0x10;
            }
            // kalo gaada file yang parentIndex dan namanya sesuai di path
            // (bakal terminate loop) ganti parentIndex jadi indeks dari
            // file/folder yang sesuai kriteria atas
            if (found) parentIndex = (i / 0x10) - 1;
        }
        j++;
    }

    return !found ? -1 : parentIndex;
}

void removeFile(char *path, int *result, char parentIndex) {
    char dir[2 * 512], map[512], sec[512];
    char emptySec[512];
    char fileName[14];
    char temp[64][14];

    int test, fileIndex, n;
    int i, j;
    int secIndex;

    getSector(map, 0x100);
    getSector(dir, 0x101);
    getSector(dir + 512, 0x102);
    getSector(sec, 0x103);

    test = getFileIndex(path, parentIndex, dir);
    if (test == -1) {
        *result = -1;
        return;
    }

    fileIndex = test & 0xFF;
    if (*(dir + fileIndex * 16 + 1) > '\x1F') {
        *result = -2;
        return;
    }

    secIndex = *(dir + (fileIndex * 16) + 1);
    j = 0;
    // hapus buffer dan file sector
    while (*(sec + (secIndex * 16) + j) != 0 && j < 16) {
        *(map + *(sec + (secIndex * 16) + j)) = 0;
        j++;
    }

    j = *(dir + fileIndex * 16 + 1);
    for (i = 0; i < 16; ++i) {
        *(dir + fileIndex * 16 + i) = 0;
        *(sec + j * 16 + i) = 0;
    }

    updateSector(map, 0x100);
    updateSector(dir, 0x101);
    updateSector(dir + 512, 0x102);
    updateSector(sec, 0x103);
}
