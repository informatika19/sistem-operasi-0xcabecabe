/**
 * kernel.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 20 Februari 2021
 */
#include "kernel.h"
#include "filesystem.h"
#include "io.h"
#include "lib/lib.h"

int main()
{
    char testo[8*1024];
    int res;
    // Set video mode
    // http://www.ctyme.com/intr/rb-0069.htm
    // 640x200 with 16 colors, 80x30 text resolution
    interrupt(0x10, 0x0012, 0, 0, 0);
    makeInterrupt21();

    readFile(testo, "math.h", &res, 0xFF);
    printString(testo);
    printString("\n");
    printLogoASCII();
}

void handleInterrupt21(int AX, int BX, int CX, int DX)
{
    char AL, AH;
    AL = (char) AX;
    AH = (char) (AX >> 8); // digeser 8 bit atau 1 byte
    switch(AL)
    {
        case 0x00:
            printString(BX);
            break;
        case 0x01:
            readString(BX);
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

void clear(char *buffer, int length)
{
    int i = 0;
    for (; i < length; ++i)
    {
        buffer[i] = 0;
    }
}

