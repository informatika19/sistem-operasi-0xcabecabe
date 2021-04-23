#include "../lib/fileIO.h"
#include "../lib/teks.h"
#include "../lib/utilities.h"

// TODO: cek yang mau di-link file apa dir
int main() {
    char cmd[10 * 20];
    char argv[5][20];
    char paths[64][14];
    char dir[1024];
    int argc;

    char buf[16 * SECTOR_SIZE];
    char *resourcePath;
    char *destinationPath;
    char cwdIdx;

    int res, test;
    char temp;

    argc = getArguments(argv);

    if (argc < 0) {
        print("Terjadi kesalahan saat mendapatkan argumen perintah.\n");
        goto error;
    }

    if (argc != 3) {
        print("Penggunaan: cp <path/ke/sumber> <path/ke/tujuan>\n");
        goto error;
    }

    // print("setelah cek argc\n");
    cwdIdx = atoi(argv[0]) & 0xFF;
    resourcePath = argv[1];
    destinationPath = argv[2];

    getSector(dir, 0x101);
    getSector(dir + 512, 0x102);
    test = getFileIndex(destinationPath, cwdIdx, dir);

    if (test != -1) {
        // artinya udah ada file dengan nama sama
        temp = test & 0xFF;
        if (temp == '\xFF' || *(dir + temp * 16 + 1) != '\xFF') {
            print(destinationPath);
            print(" sudah ada.\n");
            goto error;
        }

        // dapetin nama file buat destination path
        res = strntoken(resourcePath, paths, '/', 14);
        strncat(destinationPath, paths[res - 1], 14);
    }

    getFile(buf, resourcePath, &res, cwdIdx);
    if (res <= 0) {  // read error
        print("File ");
        print(resourcePath);
        print(" tidak ditemukan\n");
        goto error;
    }

    updateFile(buf, destinationPath, &res, cwdIdx);
    if (res <= 0) {  // write errror
        print("Terjadi kesalahan saat menyalin file.\n");
        goto error;
    }
exec_shell:
    sendArguments("", cwdIdx);
    exec("/bin/shell", 0x2000, 0, 0xFF);

error:
    goto exec_shell;
}
