/**
 * kernel.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 20 Februari 2021
 */
#include "kernel.h"
#include "lib/boolean.h"
#include "lib/math.h"
#include "lib/lib.h"
#include "lib/string.h"

int main()
{
    char testo[10], apel[10];
    char a[10], b[10];
    int anjing;
    // Set video mode
    // http://www.ctyme.com/intr/rb-0069.htm
    // 640x200 with 16 colors, 80x30 text resolution
    interrupt(0x10, 0x0012, 0, 0, 0);

    printLogoGrafik(140);
    while(1)
    {
        clear(testo, 10);
        clear(apel, 10);
        clear(a, 10);
        clear(b, 10);

        printString("Tuliskan keluh-kesahmu hari ini: ");

        a[0] = 'a';
        a[1] = 'b';
        a[2] = 'c';

        anjing = writeSector(a, 2879);
        a[0] = anjing+'0';
        a[1] = 0;
        printString("\n");
        printString(a);
        printString("\n");

        anjing = readSector(b, 2879);
        a[0] = anjing+'0';
        a[1] = 0;
        printString("\n");
        printString(a);
        printString("\n");

        readString(testo);
        /*printString(testo);*/
        /*printString("\n");*/
    }
}

void handleInterrupt21(int AX, int BX, int CX, int DX)
{
    switch(AX)
    {
        case 0x0:
            printString(BX);
            break;
        case 0x1:
            readString(BX);
            break;
        default:
            printString("Invalid interrupt");
    }
}

void printString(char *string)
{
    // Pake teletype output (basiclly yang AH=09h atau AH = 10h (?)
    // tapi bisa otomatis geser kursor dan insert new line
    // http://www.ctyme.com/intr/rb-0106.htm
    int i, baris;
    char test;
    for (i = 0; string[i] != '\0'; ++i)
    {
        switch (string[i])
        {
            // bikin new line pas ketemu \n
            case '\n':
                baris = (getCursorRow()+1)*0x100;
                interrupt(0x10, 0x0200, 0x0000, 0x0000, baris);
                break;
            default:
                interrupt(0x10, 0x0E00 + string[i], 0x0000 + WHITE, 0x0000, 0x0000);
        }
    }
}

void readString(char *string)
{
    // perlu loop -> break pas pencet enter? ASCII enter: 0xd
    // perlu merhatiin backspace, ASCII bspc: 0x8

    int i = 0; // indeks
    int kolom, baris,
        minBaris = getCursorRow(), // biar ga nimpa ke atasnya pas backspace
        minKolom = getCursorCol(),
        maxKolom = minKolom; // buat liat kolom maksimumnya berapa
    bool udah = false;

    while (1)
    {
        kolom = getCursorCol();
        baris = getCursorRow();

        kolom = (kolom <= minKolom && baris <= minBaris) ? minKolom : kolom;
        string[i] = interrupt(0x16,0x0000,0x0000,0x0000,0x0000);
        if ((string[i] == 0x8 && i>0) && !(baris == minBaris && kolom <= minKolom))
        {
            // backspace
            // idenya: mundurin cursor by 1, terus timpa karakter sblmnya dgn pixel hitam
            i--;

            if (kolom > 0)
            {
                kolom--;
            }
            else if (kolom == 0 && baris > minBaris)
            {
                baris--;
                kolom = maxKolom;
            }

            interrupt(0x10, 0x0200, 0x0000, 0x0000, baris*0x100+kolom);
            interrupt(0x10, 0x0920, 0x0000 + BLACK, 0x1, 0);

        }
        else if (string[i] == 0x0D)
        {
            printString("\n");
            break;
        }
        else if (string[i] != 0x8)
        {
            interrupt(0x10, 0x0E00 + string[i], 0x0000 + WHITE, 0x0000, 0x0000);
            i++;
            if ( !udah && baris == minBaris+1)
            {
                maxKolom += i-2; // KENAPA -2 ??????????????????????????????????
                udah = true;
            }
        }
    }
    string[i++] = 0x0; // == '\0'
}

void bikinPersegi(int sisi, int warna, int x, int y)
{
    int i, j, k = 0;
    for (i = 0; i < sisi; ++i)
    {
        for (j = 0; j < sisi; ++j)
        {
            // Pake write graphics
            // http://www.ctyme.com/intr/rb-0104.htm
            interrupt(0x10, 0x0C00 + warna, 0x0000, i+x, j+y);
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

void printLogoASCII()
{
    printString("   ___                _                    _          ");
    printString("\n");
    printString("  / _ \\__  _____ __ _| |__   ___  ___ __ _| |__   ___ ");
    printString("\n");
    printString(" | | | \\ \\/ / __/ _` | '_ \\ / _ \\/ __/ _` | '_ \\ / _ \\ ");
    printString("\n");
    printString(" | |_| |>  | (_| (_| | |_) |  __| (_| (_| | |_) |  __/ ");
    printString("\n");
    printString("  \\___//_/\\_\\___\\__,_|_.__/ \\___|\\___\\__,_|_.__/ \\___| ");
    printString("\n");
}

void printLogoGrafik(int sisi)
{
    int i, j, k = 0, y = getCursorRow(),
        setSisi = sisi/2,
        radius1 = sisi/20,
        radius2 = sisi/10,
        radius3 = sisi/15;
    // implementasinya sekarang masih lemot karena ada nimpa-nimpa pixel yang
    // udah digambar sebelumnya
    bikinPersegi(sisi, LTCYAN, 0, 0);
    for (i = setSisi-radius1; i < (setSisi+radius1); ++i)
    {
        for (j = setSisi-radius1; j < (setSisi+radius1); ++j)
        {
            bikinPersegi(1, GREEN, i+40, j-25);
        }
    }
    for (i = setSisi-radius2; i < (setSisi+radius2); ++i)
    {
        for (j = setSisi-radius2; j < (setSisi+radius2); ++j)
        {
            bikinPersegi(1, RED, i+20, j-10);
        }
    }
    for (i = setSisi-radius2; i < (setSisi+radius2); ++i)
    {
        for (j = setSisi-radius2; j < (setSisi+radius2); ++j)
        {
            bikinPersegi(1, RED, i-5, j+10);
        }
    }
    for (i = setSisi-radius3; i < (setSisi+radius3); ++i)
    {
        for (j = setSisi-radius3; j < (setSisi+radius3); ++j)
        {
            bikinPersegi(1, RED, i-25, j);
        }
    }

    interrupt(0x10, 0x0200, 0x0000, 0x0000, y+9*0x100); // buat nurunin kursor
}

int readSector(char *buffer, int sector)
{
    return interrupt(0x13, 0x0201, (int) buffer, // number, AX, BX
            div(sector, 36) * 0x100 + mod(sector, 18) + 1, // CX
            mod(div(sector, 18), 2) * 0x100); // DX
}

int writeSector(char *buffer, int sector)
{
    interrupt(0x13, 0x0301, (int) buffer,
            div(sector, 36) * 0x100 + mod(sector, 18) + 1,
            mod(div(sector, 18), 2) * 0x100);
}
