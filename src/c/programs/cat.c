// TODO: cek dia direktori atau file

#include "../lib/fileIO.h"
#include "../lib/utilities.h"

int main(){
    int argc; char argv[5][20];
    char *path;
    char cwdIdx;
    char buf[16 * SECTOR_SIZE];
    int res = 0;

    argc = getArguments(argv);
    cwdIdx = atoi(argv[0]) & 0xFF;
    path = argv[1];

    getFile(buf, path, &res, cwdIdx);

    if (res > 0)
        print(buf);
    else {
        print("Terjadi kesalahan saat membaca berkas ");
        print(path);
    }
    print("\n");

exec_shell:
        sendArguments("", cwdIdx);
        exec("/bin/shell", 0x3002, 0, 0xFF);
error:
    goto exec_shell;

}
