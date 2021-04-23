#include "../lib/boolean.h"
#include "../lib/teks.h"
#include "../lib/utilities.h"

int main() {
    char buf[16 * 64];
    char argv[5][20];
    char cwdIdx;
    bool done = false;
    int argc;

    argc = getArguments(argv);

    if (argc <= 0 || argc >= 3) {
        print("Penggunaan: echo [string]");
        goto error;
    }

    cwdIdx = atoi(argv[0]) & 0xFF;

    if (argc == 1) {
        print("Masukkan `quit` untuk keluar dari program.\n");
        while (!done) {
            print(">> ");
            read(buf);
            print(buf);
            print("\n");
            done = strncmp(buf, "quit", 4) == 0;
        }
    } else {
        print(argv[1]);
        print("\n");
    };

exec_shell:
        sendArguments("", cwdIdx);
        exec("/bin/shell", 0x2000, 0, 0xFF);
error:
    goto  exec_shell;
}