// TODO: FINISH ME

#include "../lib/fileIO.h"
#include "../lib/folderIO.h"
#include "../lib/teks.h"
#include "../lib/utilities.h"

int main(){
    int argc; char argv[5][20];

    char dir[2 * 512];

    char filename[14];
    char temp[64][14];

    char fileIndex;
    char currentIndex;

    char *path;
    char cwdIdx;

    int result;
    bool isDeletingFolder;

    argc = getArguments(argv);

    if (argc == -1) {
        print("Terjadi kesalahan saat mendapatkan argumen.\n");
        goto error;
    }

    if (argc < 2 || argc > 3) {
        print("Penggunaan: rm [-r] <path/ke/file>");
        goto error;
    }

    cwdIdx = atoi(argv[0]) & 0xFF;
    isDeletingFolder = strncmp(argv[1], "-r", 2) == 0;
    path = argv[1 + isDeletingFolder];

    // ntar bedain kalo delete file atau folder
    removeFile(path, &result, cwdIdx);
    if (result == -2) {
        if (!isDeletingFolder) {
            print(path);
            print(" adalah sebuah folder.\n");
            goto error;
        }

        removeFolder(path, &result, cwdIdx);
        if (result == 0) {
            goto exec_shell;
        }
    }

    if (result < 0) {
        print("Terjadi kesalahan saat menghapus file.\n");
        goto error;
    }

exec_shell:
    sendArguments("", cwdIdx);
    exec("/bin/shell", 0x2002, 0, 0xFF);

error:
    // print("Terjadi kesalahan saat menghapus file");
    goto exec_shell;
}