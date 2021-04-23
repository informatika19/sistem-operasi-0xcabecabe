
#include "../lib/fileIO.h"
#include "../lib/utilities.h"

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

    if (argc < 0) {
        print("Terjadi kesalahan saat mendapatkan argumen perintah.\n");
        goto error;
    }

    cwdIdx = atoi(argv[0]) & 0xFF;
    resourcePath = argv[1];
    destinationPath = argv[2];

    getSector(dir, 0x101);
    getSector(dir + 512, 0x102);
    desTest = getFileIndex(destinationPath, cwdIdx, dir);
    resTest = getFileIndex(resourcePath, cwdIdx, dir);

    if (resTest != -1) {
        // artinya udah ada file asal dengan nama sama
        temp = resTest & 0xFF;
        //kasus file destination belum ada (desTest == -1)
        if (temp == '\xFF' || *(dir + temp * 16 + 1) != '\xFF') {
            // TODO: parse file buat kasus a/b/c
            strncpy((dir + resTest*16 + 2), destinationPath, 14);
            goto exec_shell;
        }

        // dapetin nama file buat destination path
        res = strntoken(resourcePath, paths, '/', 14);
        strncat(destinationPath, paths[res - 1], 14);
    }

    exec_shell:
        sendArguments("", cwdIdx);
        exec("/bin/shell", 0x2000, 0, 0xFF);
    error:
        goto exec_shell;

}
