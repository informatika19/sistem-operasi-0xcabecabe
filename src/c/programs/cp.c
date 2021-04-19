#include "../lib/fileIO.h"
#include "../lib/teks.h"
#include "../lib/utilities.h"

// TODO: cek yang mau di-link file apa dir
int main() {
    char cmd[10 * 20];
    char argv[5][20];
    int argc;

    char buf[16 * SECTOR_SIZE];
    int res;
    char *resourcePath;
    char *destinationPath;
    char cwdIdx;

    argc = getArguments(argv);

    if (argc < 0) {
        goto error;
    }

    if (argc != 3) {
        print("Penggunaan: cp <path/ke/sumber> <path/ke/tujuan>\n");
        goto error;
    }

    cwdIdx = atoi(argv[0]) & 0xFF;
    resourcePath = argv[1];
    destinationPath = argv[2];

    getFile(buf, resourcePath, &res, cwdIdx);
    if (res <= 0) {  // read error
        print("File ");
        print(resourcePath);
        print(" tidak ditemukan\n");
        goto error;
    }

    updateFile(buf, destinationPath, &res, cwdIdx);
    if (res <= 0) {  // write errror
        goto error;
    }

exec_shell:
    sendArguments("", cwdIdx);
    exec("/bin/shell", 0x3000, 0, 0xFF);

error:
    print("Terjadi kesalahan saat menyalin file.\n");
    goto exec_shell;
}
