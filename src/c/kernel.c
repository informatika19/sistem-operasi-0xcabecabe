/**
 * kernel.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 20 Februari 2021
 *
 * Catatan:
 * - istilah direktori (dir) dan folder interchangable
 */
#include "kernel.h"

#include "filesystem.h"
#include "io.h"
#include "lib/lib.h"
#include "shell.h"

int main() {
    char buffer[512];
    int res, hadeh;
    char p[64][14], fname[14];

    // interrupt(0x10, 0x0003, 0, 0, 0);
    // Set video mode
    // http://www.ctyme.com/intr/rb-0069.htm
    // 640x200 with 16 colors, 80x30 text resolution
    interrupt(0x10, 0x0012, 0, 0, 0);
    makeInterrupt21();

    strncpy(buffer, "Tolong, saya shinitai-desu:(", 70);
    res = 1;
    writeFile(buffer, "/test.txt", &res, 0xFF);
    clear(buffer, 70);

    strncpy(buffer, "hadeh", 10);
    res = 1;
    writeFile(buffer, "/testa2.txt", &res, 0xFF);

    printLogoGrafik(140);
    runShell();

    while (true)
        ;
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
    char AL, AH;
    AL = (char)AX;
    AH = (unsigned char)(AX >> 8);  // digeser 8 bit atau 1 byte
    switch (AL) {
        case 0x00:
            printString(BX);
            break;
        case 0x01:
            readString(BX);
            break;
        case 0x02:
            readSector(BX, CX);
            break;
        case 0x03:
            writeSector(BX, CX);
            break;
        case 0x04:
            readFile(BX, CX, DX, AH);
            break;
        case 0x05:
            writeFile(BX, CX, DX, AH);
            break;
        default:
            printString("Invalid interrupt");
    }
}

void clear(char *buffer, int length) {
    int i = 0;
    for (; i < length; ++i) {
        buffer[i] = 0;
    }
}
