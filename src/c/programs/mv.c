
#include "../lib/fileIO.h"
#include "../lib/utilities.h"
#include "../lib/teks.h"

int main() {
    // mv asal tujuan
    int argc; char argv[5][20];
    char *dest;
    char *src;
    char cwdIdx;
    char dir[1024];
    char buf[16 * SECTOR_SIZE];
    char paths[64][14];
    int res = 0;
    char *resourcePath;
    char *destinationPath;
    char temp;
    char *name;
    int desTest;
    int resTest;

    argc = getArguments(argv);

    if (argc != 3) {
        print("Penggunaan: mv <path/ke/sumber> <path/ke/tujuan>\n");
        goto error;
    }

    cwdIdx = atoi(argv[0]) & 0xFF;
    if (argc < 0) {
        print("Terjadi kesalahan saat mendapatkan argumen perintah.\n");
        goto error;
    }

    resourcePath = argv[1];
    destinationPath = argv[2];

    if (strncmp(resourcePath, destinationPath, 14) == 0) {
        print(resourcePath);
        print(" dan ");
        print(destinationPath);
        print(" adalah file yang sama.");
        goto error;
    }

    getSector(dir, 0x101);
    getSector(dir + 512, 0x102);
    desTest = getFileIndex(destinationPath, cwdIdx, dir);
    resTest = getFileIndex(resourcePath, cwdIdx, dir);

    // file sumber tidak ada
    if (resTest == -1) {
        print(resourcePath);
        print(" tidak ada.\n");
        goto error;
    }

    // artinya udah ada file tujuan dengan nama sama
    // "benerin" destination path
    if (desTest != -1) {
        temp = desTest & 0xFF;

        if (temp != '\xFF' && *(dir + temp * 16 + 1) != '\xFF') {
            // destination path = directory (copy abis tu remove)
            // TODO: parse file buat kasus a/b/c
            print(destinationPath);
            print(" sudah ada.\n");
            goto error;
        }

        // dapetin nama file buat destination path
        res = strntoken(resourcePath, paths, '/', 14);
        strncat(destinationPath, "/", 1);
        strncat(destinationPath, paths[res - 1], 14);
    }

    getFile(buf, resourcePath,&res,cwdIdx);
    if (res <= 0) {
        print(resourcePath);
        print(" tidak ada.\n");
        goto error;
    }
    updateFile(buf,destinationPath,&res,cwdIdx);
    if (res <= 0) {
        print("Terjadi kesalahan saaat memindahkan file.\n");
        goto error;
    }
    removeFile(resourcePath, &res, cwdIdx);
    if (res <= 0) {  // write errror
        switch (res) {
            case -1:
                print(destinationPath);
                print(" sudah ada.\n");
                break;
            case -2:
            case -3:
                print("File system penuh.\n");
                break;
            case -4:
                print(destinationPath);
                print(" invalid.\n");
                break;
            default:
                print("Terjadi kesalahan saat menyalin file.\n");
        }
        goto error;
    }

    exec_shell:
        sendArguments("", cwdIdx);
        exec("/bin/shell", 0x2000, 0, 0xFF);
    error:
        goto exec_shell;

}
