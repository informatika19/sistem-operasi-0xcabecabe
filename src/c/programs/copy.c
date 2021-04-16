#include "../lib.h"

// TODO: cek yang mau di-link file apa dir
int main() {
    int argc; char argv[5][20];

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
        return -1;
    }

    writeFile(buf, destinationPath, &res, cwdIdx);
    if (res <= 0) {  // write errror
        goto cp_error;
        return -1;
    }

    return -1;

cp_error:
    printString("Terjadi kesalahan saat menyalin file.\n");
    return -1;
}
