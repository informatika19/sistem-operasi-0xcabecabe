/**
 * kernel.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 20 Februari 2021
 */

#include "kernel.h"

#include "lib/fileIO.h"
#include "lib/io.h"
#include "lib/lib.h"

int main() {
    char buffer[512];
    int res;

    // interrupt(0x10, 0x0003, 0, 0, 0);
    makeInterrupt21();
    // Set video mode
    // http://www.ctyme.com/intr/rb-0069.htm
    // 640x200 with 16 colors, 80x30 text resolution
    interrupt(0x10, 0x0012, 0, 0, 0);
    printLogoGrafik(140);
    printString("Pedas OS by 0xCABECABE\n");
    printString("press enter to continue...");
    readString(0);
    interrupt(0x10, 0x0012, 0, 0, 0);

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

/*
void printLogoASCII() {
    int y = getCursorRow();
    printString("   ___                _                    _          ");
    printString("\n");
    printString("  / _ \\__  _____ __ _| |__   ___  ___ __ _| |__   ___ ");
    printString("\n");
    printString(
    " | | | \\ \\/ / __/ _` | '_ \\ / _ \\/ __/ _` | '_ \\ / _ \\ ");
    printString("\n");
    printString(" | |_| |>  | (_| (_| | |_) |  __| (_| (_| | |_) |  __/ ");
    printString("\n");
    printString(
    "  \\___//_/\\_\\___\\__,_|_.__/ \\___|\\___\\__,_|_.__/ \\___| ");
    printString("\n");
    interrupt(0x10, 0x0200, 0x0000, 0x0000,
    y + 6 * 0x100);  // buat nurunin kursor
}
*/

void printLogoGrafik(int sisi) {
    int i, j, y = getCursorRow(), setSisi = sisi / 2, radius1 = sisi / 20,
              radius2 = sisi / 10, radius3 = sisi / 15;
    // implementasinya sekarang masih lemot karena ada nimpa-nimpa pixel yang
    // udah digambar sebelumnya
    bikinPersegi(sisi, LTCYAN, 0, 0);
    for (i = setSisi - radius1; i < (setSisi + radius1); ++i) {
        for (j = setSisi - radius1; j < (setSisi + radius1); ++j) {
            bikinPersegi(1, GREEN, i + 40, j - 25);
        }
    }
    for (i = setSisi - radius2; i < (setSisi + radius2); ++i) {
        for (j = setSisi - radius2; j < (setSisi + radius2); ++j) {
            bikinPersegi(1, RED, i + 20, j - 10);
        }
    }
    for (i = setSisi - radius2; i < (setSisi + radius2); ++i) {
        for (j = setSisi - radius2; j < (setSisi + radius2); ++j) {
            bikinPersegi(1, RED, i - 5, j + 10);
        }
    }
    for (i = setSisi - radius3; i < (setSisi + radius3); ++i) {
        for (j = setSisi - radius3; j < (setSisi + radius3); ++j) {
            bikinPersegi(1, RED, i - 25, j);
        }
    }

    interrupt(0x10, 0x0200, 0x0000, 0x0000,
              y + 9 * 0x100);  // buat nurunin kursor
}

void readSector(char *buffer, int sector) {
    interrupt(0x13, 0x0201, buffer,                           // number, AX, BX
              div(sector, 36) * 0x100 + mod(sector, 18) + 1,  // CX
              mod(div(sector, 18), 2) * 0x100);               // DX
}

void writeSector(char *buffer, int sector) {
    interrupt(0x13, 0x301, buffer,
              div(sector, 36) * 0x100 + mod(sector, 18) + 1,
              mod(div(sector, 18), 2) * 0x100);
}
