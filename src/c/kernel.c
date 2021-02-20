#include "kernel.h"
#include "lib.h"

int main()
{
    makeInterrupt21();

    // Set video mode
    // http://www.ctyme.com/intr/rb-0069.htm
    // Graphics mode, 320x200 with 256 colors, 40x25 text resolution
    /*interrupt(0x10, 0x0013, 0, 0, 0);*/
    // Text mode, Basically 640x200 with 16 colors, 80x25 text resolution
    interrupt(0x10, 0x0003, 0, 0, 0);

    printString("The quick brown fox jumps over the lazy dog lorem ipsum dor amet hello world");
    /*bikinPersegi(128, LTCYAN);*/
    while(1);
}

void handleInterrupt21(int AX, int BX, int CX, int DX)
{
    switch(AX)
    {
        case 0x0:
            printString((char *) BX);
            break;
        /*case 0x1:*/
            /*printString((char *) BX);*/
            /*break;*/
        default:
            printString("Invalid interrupt");
    }
}

void printString(char *string)
{
    // Pake teletype output (basiclly yang AH=09h atau AH = 10h (?)
    // tapi bisa otomatis geser kursor dan insert new line
    // http://www.ctyme.com/intr/rb-0106.htm
    int i = 0;
    while (string[i] != '\0')
    {
        interrupt(0x10, 0x0E00 + string[i], 0x0000 + WHITE, 0x0000, 0x0000);
        i++;
    }
}

void bikinPersegi(int sisi, int warna)
{
    int i, j, k = 0;
    for (i = 0; i < sisi; ++i)
    {
        for (j = 0; j < sisi; ++j)
        {
            // Pake write graphics
            // http://www.ctyme.com/intr/rb-0104.htm
            interrupt(0x10, 0x0C00 + warna, 0x0000, i, j);
        }
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
