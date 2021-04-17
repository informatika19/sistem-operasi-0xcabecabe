#include "../lib.h"

// TODO: cek yang mau di-link file apa dir
int main() {
    char cmd[10 * 20];
    int argc; char argv[5][20];

    char buf[16 * SECTOR_SIZE];
    int res;
    char *resourcePath;
    char *destinationPath;
    char cwdIdx = atoi(argv[3])&0xFF;

    readFile(cmd, "/argv.tmp", &res, 0xFF);
    if (res < 0) {
        printString("Gagal menjalankan perintah cp.");
        return -1;
    }

    printString("hadeh\n");

    argc = strntoken(cmd, argv, ' ', 20);

    if (argc != 3) {
        printString("Penggunaan: cp <path/ke/sumber> <path/ke/tujuan>\n");
        return -1;
    }

    cwdIdx = *argv[0];
    resourcePath = argv[1];
    destinationPath = argv[2];

    readFile(buf, resourcePath, &res, cwdIdx);
    if (res <= 0) {  // read error
        printString("File ");
        printString(resourcePath);
        printString(" tidak ditemukan\n");
        // executeProgram("/bin/shell", 0x5200, 0, 0xFF);
        return -1;
    }

    writeFile(buf, destinationPath, &res, cwdIdx);
    if (res <= 0) {  // write errror
        goto cp_error;
        // executeProgram("/bin/shell", 0x5200, 0, 0xFF);
        return -1;
    }

    // executeProgram("/bin/shell", 0x5200, 0, 0xFF);
    return 0;

cp_error:
    printString("Terjadi kesalahan saat menyalin file.\n");
    // executeProgram("/bin/shell", 0x5200, 0, 0xFF);
    return -1;
}
