/**
 * kernel.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 20 Februari 2021
 */

#include "kernel.h"

#include "lib.h"

int main() {
    int res;
    makeInterrupt21();
    // Set video mode
    // http://www.ctyme.com/intr/rb-0069.htm
    interrupt(0x10, 0x0012, 0, 0, 0);
    printLogoGrafik(140);
    printString("Pedas OS by 0xCABECABE\n");
    printString("press enter to continue...");
    readString(0);
    interrupt(0x10, 0x0012, 0, 0, 0);

    res = 1;

    writeFile("255", "argv.tmp", &res, 0xFF);

    executeProgram("/bin/shell", 0x3000, 0, 0xFF);

    printString("Otw gila\n");
    while (true)
        ;
}

void clear(char *buffer, int length) {
    int i = 0;
    for (; i < length; ++i) {
        buffer[i] = 0;
    }
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
        case 0x06:
            executeProgram(BX, CX, DX, AH);
        default:
            printString("Invalid interrupt");
    }
}

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

void executeProgram(char *filename, int segment, int *success,
                    char parentIndex) {
    // Buat buffer
    int isSuccess;
    char fileBuffer[512 * 16];
    int i = 0;
    // Buka file dengan readFile
    readFile(&fileBuffer, filename, &isSuccess, parentIndex);
    // If success, salin dengan putInMemory
    if (isSuccess > 0) {
        // launchProgram
        for (i = 0; i < 512 * 16; i++) {
            putInMemory(segment, i, fileBuffer[i]);
        }
        *success = true;
        launchProgram(segment);
    } else {
        *success = false;
        interrupt(0x21, 0, "File not found!\n", 0, 0);
    }
}

void writeFile(char *buffer, char *path, int *sectors, char parentIndex) {
    int i, j, entry, sectorNeeded, sectorFree = 0, sectorsToUse[16],
                                   entrySectors;
    bool alreadyExists = false, parentExists = (parentIndex == '\xFF');
    char map[SECTOR_SIZE], dir[2 * SECTOR_SIZE], sec[SECTOR_SIZE];
    char fileName[14], parents[64][14];
    /*
    -1 file sudah ada
    -2 tidak cukup entri di sektor files
    -3 tidak cukup sektor kosong
    -4 folder tidak valid
    */

    if (*sectors > 16) {
        *sectors = -3;
        return;
    }

    sectorNeeded = *sectors;
    readSector(map, 0x100);
    readSector(dir, 0x101);
    readSector(dir + SECTOR_SIZE, 0x102);
    readSector(sec, 0x103);

    j = strntoken(path, parents, '/', 14);
    strncpy(fileName, parents[--j], 14);

    // adjust parent index ke index tujuan
    // hapus element terakhir dari parents
    if (j != 0) {
        clear(path, strlen(path));
        strncpy(path, parents[0], strlen(parents[0]));
        strncat(path, "/", 14);
        for (i = 1; i < j; ++i) {
            strncat(path, parents[i], strlen(parents[i]));
            strncat(path, "/", 2);
        }
        parentIndex = getFileIndex(path, parentIndex, dir) & 0xFF;
    }

    // akibat dari path yang diberikan tidak valid
    if (parentIndex < 0) {
        *sectors = -4;
        return;
    }

    // ngecek file dengan yang sama di parent index yang sama udah ada atau
    // belum
    i = 0;
    while (i < 2 * SECTOR_SIZE && !alreadyExists) {
        parentExists = !parentExists ? *(dir + i) == parentIndex : parentExists;
        alreadyExists = *(dir + i) == parentIndex &&
                        strncmp(dir + i + 2, fileName, 13) == 0;
        i += 0x10;
    }
    if (alreadyExists) {
        *sectors = -1;
        return;
    }
    if (!parentExists) {
        *sectors = -4;
        return;
    }

    // nyari entri kosong di sektor files yang kosong kosong
    entry = 0;
    for (; *(dir + entry + 2) && entry < 2 * SECTOR_SIZE; entry += 0x10)
        ;
    if (entry >= 2 * SECTOR_SIZE) {
        *sectors = -2;
        return;
    }

    // nyari sektor-sektor di map yang kosong untuk diisi file
    // sektor yang digunakan bisa tidak kontigu
    i = 0;
    j = 0;
    while (i < SECTOR_SIZE) {
        if (*(map + i) == 0x00) {
            sectorFree++;
            if (j < 16) sectorsToUse[j++] = i;
        }
        ++i;
    }
    if (sectorFree < sectorNeeded) {
        *sectors = -3;
        return;
    }

    // isi sektornya
    i = 0;
    while (i < sectorNeeded) {
        writeSector(buffer + (i * SECTOR_SIZE), sectorsToUse[i]);
        *(map + sectorsToUse[i]) = 0xFF;
        ++i;
    }

    // isi sektor sectors (0x103)
    // cari entri di sektor sectors yang kosong
    entrySectors = 0;
    for (; *(sec + (entrySectors)) && entrySectors < SECTOR_SIZE;
         entrySectors += 0x10)
        ;

    *sectors = 0;
    for (i = 0; i < sectorNeeded; ++i, ++(*sectors))
        *(sec + entrySectors + i) = sectorsToUse[i];

    // akusisi entry yang ditemukan sebelumnya
    *(dir + entry) = parentIndex;
    *(dir + entry + 1) = entrySectors / 0x10;
    strncpy(dir + entry + 2, fileName, 14);

    // tulis perubahan
    writeSector(map, 0x100);
    writeSector(dir, 0x101);
    writeSector(dir + SECTOR_SIZE, 0x102);
    writeSector(sec, 0x103);
}

void readFile(char *buffer, char *path, int *result, char parentIndex) {
    int i, test;
    char dir[2 * SECTOR_SIZE], sec[SECTOR_SIZE];
    char *entry, secIdx, *secNo;
    /*
    -1 file tidak ditemukan
    */

    readSector(dir, 0x101);
    readSector(dir + SECTOR_SIZE, 0x102);
    readSector(sec, 0x103);

    // file tidak ditemukan di parent atau parent tidak ada
    test = getFileIndex(path, parentIndex, dir);
    if (test == -1) {
        *result = -1;
        return;
    }
    i = test & 0xFF;
    entry = dir + (i * 0x10);

    // bukan file
    if (*(entry + 1) > 0x1F) {
        *result = -1;
        return;
    }

    i = 0;
    secIdx = *(entry + 1);
    secNo = sec + (secIdx * 0x10);
    while (*secNo && i < 0x10) {
        readSector(buffer + (i * SECTOR_SIZE), *secNo);
        i++;
        secNo++;
    }

    *(buffer + (i * SECTOR_SIZE)) = 0;
    *result = i;
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
