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
#include "lib.h"

int main()
{
    char testo[8*1024];
    char parents[64][14]; // jadi flat array dengan jarak antarelemen 14
    char buffer[512];
    char filename[14];
    int res, hadeh;
    // Set video mode
    // http://www.ctyme.com/intr/rb-0069.htm
    // 640x200 with 16 colors, 80x30 text resolution
    interrupt(0x10, 0x0012, 0, 0, 0);
    clear(buffer, 512);
    makeInterrupt21();

    strncpy(buffer, "tolong saya\n", 12);
    res = 1;
    writeFile(buffer, "/test.txt", &res, 0xFF);
    if (res > 0)
    {
        printString("Berhasil nulis :D\n");
        readFile(testo, "/test.txt", &hadeh, 0xFF);
        if (hadeh > 0)
            printString(testo);
        else
            printString("ga bisa baca :(\n");
    }
    else
    {
        printString("menghangdeh: -");
        hadeh = -1 * res;
        printNumber(hadeh);
        printString("\n");
    }

    strncpy(buffer, "sini saya tolong\n", 17);
    res = 1;
    writeFile(buffer, "/test2.txt", &res, 0xFF);
    if (res > 0)
    {
        printString("Berhasil nulis :D\n");
        readFile(testo, "/test2.txt", &hadeh, 0xFF);
        if (hadeh > 0)
            printString(testo);
        else
            printString("ga bisa baca :(\n");
    }
    else
    {
        printString("menghangdeh: -");
        hadeh = -1 * res;
        printNumber(hadeh);
        printString("\n");
    }

    // /usr/share/gcc-10.2.0/python/libstdcxx/__init__.py
    res = parsePath("/usr/share/../share/gcc-10.2.0//..//gcc-10.2.0/./././../gcc-10.2.0/python/libstdcxx/__init__.py", parents, filename);
    printString(filename);
    printString("\n");
    while (res--)
    {
        printString(parents[res]);
        printString("\n");
    }

    while (true);
}

void handleInterrupt21(int AX, int BX, int CX, int DX)
{
    char AL, AH;
    AL = (char) AX;
    AH = (unsigned char) (AX >> 8); // digeser 8 bit atau 1 byte
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
