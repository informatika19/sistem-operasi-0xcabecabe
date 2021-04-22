// TODO: cek yang mau di-link file apa dir

#include "../lib.h"
int main() {
    int argc; char argv[5][20];

    char dir[2 * SECTOR_SIZE];

    int testDI, testRI, i = 0, jmlParents = 0;
    char destinationIndex, resourceIndex;
    char fname[14];
    char parents[64][14];
    int tmp = 2 * SECTOR_SIZE;

    char *resourcePath;
    char *destinationPath;
    char cwdIdx;

    argc = getArguments(argv);

    if (argc < 0) {
        print("Terjadi kesalahan saat mendapatkan argumen perintah.\n");
        goto error;
    }

    if (argc != 3) {
        print("Penggunaan: ln <path/ke/sumber> <path/ke/tujuan>\n");
        goto error;
    }

    cwdIdx = atoi(argv[0]) & 0xFF;
    resourcePath = argv[1];
    destinationPath = argv[2];

    getSector(dir, 0x101);
    getSector(dir + SECTOR_SIZE, 0x102);

    testDI = getFileIndex(destinationPath, cwdIdx, dir);
    testRI = getFileIndex(resourcePath, cwdIdx, dir);
    destinationIndex = testDI & 0xFF;
    resourceIndex = testRI & 0xFF;

    if (testDI == -1 && testRI != -1) {
        jmlParents = strntoken(destinationPath, parents, '/', 14);
        strncpy(fname, parents[--jmlParents], 14);
        if (jmlParents != 0) {
            fillBuffer(destinationPath, strlen(destinationPath), 0);
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
            print("sektor penuh\n");
            goto error;
        }

        *(dir + i) = cwdIdx;
        *(dir + i + 1) = *(dir + resourceIndex * 0x10 + 1);
        strncpy(dir + i + 2, fname, 14);

        updateSector(dir, 0x101);
        updateSector(dir + SECTOR_SIZE, 0x102);

        goto exec_shell;

exec_shell:
        sendArguments("", cwdIdx);
        exec("/bin/shell", 0x3002, 0, 0xFF);

    } else {
        if (testDI != -1) {
            print(destinationPath);
            print(" sudah ada.\n");
        }
        if (testRI == -1) {
            print(resourcePath);
            print(" tidak ada.\n");
        }
        goto error;
    }

error:
    // print("Terjadi kesalahan saat membuat symbolic link\n");
    goto exec_shell;
}
