// TODO: cek dia direktori atau file

#include "../lib/fileIO.h"
#include "../lib/teks.h"
#include "../lib/utilities.h"

int main() {
    char argv[5][20];
    char *path;
    char cwdIdx;
    char buf[16 * SECTOR_SIZE];
    int argc;
    int res = 0;
    char cmd[512];

    argc = getArguments(argv);
    cwdIdx = atoi(argv[0]) & 0xFF;
    if (argc != 2) {
        print("Penggunaan: cat <file>\n");
        goto error;
    }

    path = argv[1];

    getFile(buf, path, &res, cwdIdx);

    if (res > 0) {
        print(buf);
        print("\n");
    } else {
        print("Terjadi kesalahan saat membaca berkas ");
        print(path);
        print("\n");
        goto error;
    }

exec_shell:
    sendArguments("", cwdIdx);
    exec("/bin/shell", 0x2000, 0, 0xFF);

error:
    goto exec_shell;

}
