/**
 * folderIO.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 2 April 2021
 *
 * Implementasi folderIO.h
 */

#include "folderIO.h"
#include "fileIO.h"
#include "utilities.h"
#include "teks.h"

void createFolder(char *fileName, int *success, char parentIndex) {
    char dir[2 * 512];
    char emptySec[512];
    char temp[64][14];
    char name;

    int test, fileIndex;
    int i;

    test = getFileIndex(fileName, parentIndex, dir);
    if (test != -1) {
        *success = -1;
        return;
    }

    getSector(dir, 0x101);
    getSector(dir + 512, 0x102);

    // nyari tempat kosong di sector
    for (i=0;i * 16 <SECTOR_SIZE;i++) {
        if (*(dir + i * 16 + 2) == 0) {
            break;
        }
    }

    // (test) FF (nama file)
    *(dir + i * 16) = parentIndex;
    *(dir + i * 16 + 1) = 0xFF;
    strncpy(dir + i * 16 + 2, fileName, 14);

    updateSector(dir, 0x101);
    updateSector(dir + 512, 0x102);

    *success = 0;
}

void removeFolder(char *path, int *success, char parentIndex) {
    char dir[2 * 512];
    char emptySec[512];
    char fileName[14];
    char temp[64][14];
    char childIndex;
    char *childName;

    int test, fileIndex, n;
    int i, j;
    int secIndex;
    int countChild = 0;

    getSector(dir, 0x101);
    getSector(dir + 512, 0x102);

    test = getFileIndex(path, parentIndex, dir);
    if (test == -1) {
        *success = -1;
        return;
    }

    fileIndex = test & 0xFF;
    *success = 0;
    if (*(dir + fileIndex * 16 + 1) > '\x1F') {
        j = 0;
        while(j < 64 && *success >= 0){
            if (*(dir + j * 16) == fileIndex){
                childName = dir + j * 16 + 2;
                if(*(dir + j * 16 + 1) > '\x1F'){
                    removeFolder(childName, success, fileIndex);
                } else {
                    removeFile(childName, success, fileIndex);
                }
            }
            j++;

            getSector(dir, 0x101);
            getSector(dir + 512, 0x102);
        }
    }

    if (*success == -1) {
        return;
    }

    fillBuffer(dir + fileIndex * 16, 16, 0);

    updateSector(dir, 0x101);
    updateSector(dir + 512, 0x102);
}