#include "../lib.h"

// TODO: cek yang mau di-link file apa dir
int main(char *argv, int argc) {
    char buf[16 * SECTOR_SIZE];
    char dir[2 * SECTOR_SIZE];
    int res = 0;
    char *resourcePath = argv[1];
    char *destinationPath = argv[2];
    char cwdIdx = atoi(argv[3])&0xFF;


    readSector(dir, 0x101);
    readSector(dir + SECTOR_SIZE, 0x102);

    readFile(buf, resourcePath, &res, cwdIdx);
    if (res <= 0) {  // read error
        printString("File ");
        printString(resourcePath);
        printString(" tidak ditemukan\n");
        return;
    }

    writeFile(buf, destinationPath, &res, cwdIdx);
    if (res <= 0) {  // write errror
        goto cp_error;
        return;
    }

    return;

cp_error:
    printString("Terjadi kesalahan saat menyalin file.\n");
    return;
}