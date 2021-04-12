// TODO: cek yang mau di-link file apa dir

#include "../lib.h"
int main(char *argv, int argc) {
    char dir[2 * SECTOR_SIZE];

    int testDI, testRI, i = 0, jmlParents = 0;
    char destinationIndex, resourceIndex;
    char fname[14];
    char parents[64][14];
    int tmp = 2 * SECTOR_SIZE;
    char *resourcePath = argv[1];
    char *destinationPath = argv[2];
    char cwdIdx = atoi(argv[3])&0xFF;

    readSector(dir, 0x101);
    readSector(dir + SECTOR_SIZE, 0x102);
    testDI = getFileIndex(destinationPath, cwdIdx, dir);
    testRI = getFileIndex(resourcePath, cwdIdx, dir);
    destinationIndex = testDI & 0xFF;
    resourceIndex = testRI & 0xFF;

    if (testDI == -1 && testRI != -1) {
        jmlParents = tokenize(destinationPath, parents, '/');
        strncpy(fname, parents[--jmlParents], 14);
        if (jmlParents != 0) {
            clear(destinationPath, strlen(destinationPath));
            strncpy(destinationPath, parents[0], strlen(parents[0]));
            strncat(destinationPath, "/", 14);
            for (i = 1; i < jmlParents; ++i) {
                strncat(destinationPath, parents[i], strlen(parents[i]));
                strncat(destinationPath, "/", 2);
            }
            cwdIdx = getFileIndex(destinationPath, cwdIdx, dir) & 0xFF;
        }

        i = 0;
        while (*(dir + i + 2) != 0 && i < tmp) {
            i += 0x10;
        }

        if (*(dir + i + 2) != 0) {  // sektor files penuh
            printString("sektor penuh\n");
            goto hardLink_error;
            return;
        }

        *(dir + i) = cwdIdx;
        *(dir + i + 1) = *(dir + resourceIndex * 0x10 + 1);
        strncpy(dir + i + 2, fname, 14);

        writeSector(dir, 0x101);
        writeSector(dir + SECTOR_SIZE, 0x102);

        return;
    } else {
        goto hardLink_error;
        return;
    }

hardLink_error:
    printString("Terjadi kesalahan saat membuat symbolic link\n");
    return;
}
