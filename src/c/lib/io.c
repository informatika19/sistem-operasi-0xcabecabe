/**
 * io.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 13 Maret 2021
 *
 * implementasi io.h
 */

#include "io.h"

#include "lib_asm.h"
#include "boolean.h"
#include "math.h"
#include "fileIO.h"

void printString(char *string) {
    // Pake teletype output (basiclly yang AH=09h atau AH = 10h (?)
    // tapi bisa otomatis geser kursor dan insert new line
    // http://www.ctyme.com/intr/rb-0106.htm
    int i /*, baris*/;
    for (i = 0; string[i] != '\0'; ++i) {
        switch (string[i]) {
            // bikin new line pas ketemu \n
            case '\n':
                // https://stackoverflow.com/questions/28839568/using-bios-interrupts-in-x86
                interrupt(0x10, 0x0E00 + '\r', 0x0000 + WHITE, 0x0000, 0x0000);
                interrupt(0x10, 0x0E00 + '\n', 0x0000 + WHITE, 0x0000, 0x0000);
                break;
            default:
                interrupt(0x10, 0x0E00 + string[i], 0x0000 + WHITE, 0x0000,
                          0x0000);
        }
    }
}

void readString(char *string) {
    // perlu loop -> break pas pencet enter? ASCII enter: 0xd
    // perlu merhatiin backspace, ASCII bspc: 0x8

    int i = 0;  // indeks
    int kolom, baris,
        minBaris = getCursorRow(),  // biar ga nimpa ke atasnya pas backspace
        minKolom = getCursorCol(),
        maxKolom = minKolom;  // buat liat kolom maksimumnya berapa
    bool udah = false;

    while (1) {
        kolom = getCursorCol();
        baris = getCursorRow();

        kolom = (kolom <= minKolom && baris <= minBaris) ? minKolom : kolom;
        string[i] = interrupt(0x16, 0x0000, 0x0000, 0x0000, 0x0000);
        if ((string[i] == 0x8 && i > 0) &&
            !(baris == minBaris && kolom <= minKolom)) {
            // backspace
            // idenya: mundurin cursor by 1, terus timpa karakter sblmnya dgn
            // pixel hitam
            i--;

            if (kolom > 0) {
                kolom--;
            } else if (kolom == 0 && baris > minBaris) {
                baris--;
                kolom = maxKolom;
            }

            interrupt(0x10, 0x0200, 0x0000, 0x0000, baris * 0x100 + kolom);
            interrupt(0x10, 0x0920, 0x0000 + BLACK, 0x1, 0);

        } else if (string[i] == 0x0D) {
            string[i] = '\0';
            printString("\n");
            break;
        } else if (string[i] != 0x8) {
            interrupt(0x10, 0x0E00 + string[i], 0x0000 + WHITE, 0x0000, 0x0000);
            i++;
            if (!udah && baris == minBaris + 1) {
                maxKolom +=
                    i - 2;  // KENAPA -2 ??????????????????????????????????
                udah = true;
            }
        }
    }
    string[i++] = 0x0;  // == '\0'
}

void bikinPersegi(int sisi, int warna, int x, int y) {
    int i, j;
    for (i = 0; i < sisi; ++i) {
        for (j = 0; j < sisi; ++j) {
            // Pake write graphics
            // http://www.ctyme.com/intr/rb-0104.htm
            interrupt(0x10, 0x0C00 + warna, 0x0000, i + x, j + y);
        }
    }
}

void printNumber(int number) {
    char c;

    if (number < 0) {
        printString("-");
        number *= -1;
    }

    if (number < 10) {
        c = number + '0';
        interrupt(0x10, 0x0E00 + c, 0x0000 + WHITE, 0x0000, 0x0000);
    } else {
        printNumber(div(number, 10));
        printNumber(mod(number, 10));
    }
}

void clear(char *buffer, int length) {
    int i = 0;
    for (; i < length; ++i) {
        buffer[i] = 0;
    }
}

void executeProgram(char *filename, int segment, int *success,
                    char parentIndex) {
    // Buat buffer
    int isSuccess;
    char fileBuffer[512 * 16];
    // Buka file dengan readFile
    readFile(&fileBuffer, filename, &isSuccess, parentIndex);
    // If success, salin dengan putInMemory
    if (isSuccess > 0) {
        // launchProgram
        int i = 0;
        for (i = 0; i < 512 * 16; i++) {
            putInMemory(segment, i, fileBuffer[i]);
        }
        launchProgram(segment);
    } else {
        interrupt(0x21, 0, "File not found!\n", 0, 0);
    }
}