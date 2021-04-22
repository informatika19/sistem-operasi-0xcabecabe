
#include "../lib/fileIO.h"
#include "../lib/utilities.h"

int main() {
    // mv asal tujuan
    int argc; char argv[5][20];
    char *dest;
    char *src;
    char cwdIdx;
    char buf[16 * SECTOR_SIZE];
    int res = 0;

    argc = getArguments(argv);
    cwdIdx = atoi(argv[0]) & 0xFF;
    dest = argv[1];
    src = argv[2];

    if (argc != 3) {
        print("Penggunaan: mv <path/ke/sumber> <path/ke/tujuan>\n");
        goto error;
    }

    exec_shell:
        sendArguments("", cwdIdx);
        exec("/bin/shell", 0x2002, 0, 0xFF);
    error:
        goto exec_shell;

}