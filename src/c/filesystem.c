/**
 * filesystem.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 13 Maret 2021
 *
 * implementasi filesystem.h
 */

#include "filesystem.h"
#include "kernel.h"
#include "io.h"
#include "lib.h"

int parsePath(char *path, char *parents, char *fname)
{
    int i, j;
    char cur[14];
    char testo[2];

    i = 0, j = 0;
    path = path + (1 * (*path == '/'));
    while (*path != '\0')
    {
        switch (*path) {
            case '/':
                *(parents+j+i) = 0;
                i = 0;
                j += 14;
                break;
            default:
                *(parents+j+i) = *path;
                i++;
        }
        path++;
    }
    *(parents+j+i) = 0;
    strncpy(fname, parents+j, 14);
    printString(fname);
    printString("\n");

    return div(j, 14);
}

void readSector(char *buffer, int sector)
{
    interrupt(0x13, 0x0201, buffer, // number, AX, BX
            div(sector, 36) * 0x100 + mod(sector, 18) + 1, // CX
            mod(div(sector, 18), 2) * 0x100); // DX
}

void writeSector(char *buffer, int sector)
{
    interrupt(0x13, 0x301, buffer,
            div(sector, 36) * 0x100 + mod(sector, 18) + 1,
            mod(div(sector, 18), 2) * 0x100);
}

void writeFile(char *buffer, char *path, int *sectors, char parentIndex)
{
    int i, dirSectorUsed, mapSectorCount, writeSectorCount;
    char map[SECTOR_SIZE],
        dir1[SECTOR_SIZE],
        dir2[SECTOR_SIZE],
        dir[2*SECTOR_SIZE];
    /*
    -1 file sudah ada
    -2 tidak cukup entri di files
    -3 tidak cukup sektor kosong
    -4 folder tidak valid
    */

    writeSectorCount = *sectors;
    readSector(map, 0x100);
    readSector(dir1, 0x101);
    readSector(dir2, 0x102);

    for (i = 0; i < 2*SECTOR_SIZE; ++i)
        dir[i] = dir1[i] * (i < SECTOR_SIZE) + dir2[i-512] * (i >= SECTOR_SIZE);

    i = 0;
    // nyari sektor kosong
    while (dir[i] && i < 2*SECTOR_SIZE)
        i++;
    if (i >= 2*SECTOR_SIZE)
    {
        *sectors = -2;
        return;
    }
    dirSectorUsed = i;

    i = 0;
    // periksa map yang masih muat nampung buffer atau ngga
    while (map[i] && i < SECTOR_SIZE)
        i++;
    if (writeSectorCount > (SECTOR_SIZE - i)) // tidak cukup sektor kosong
    {
        *sectors = -3;
        return;
    }
    mapSectorCount = i;

    clear(map+mapSectorCount, writeSectorCount);

    // ini di akhir
    *sectors = 0;
}

// bikin parsing
void readFile(char *buffer, char *path, int *result, char parentIndex)
{
    int ROW_SIZE = 0xF;
    int i, banyakParent;
    bool success;
    char dir[2*SECTOR_SIZE], sec[SECTOR_SIZE];
    char *fileName, *parentDirs[0x3E][14]; // array of strings untuk parent dari path
    char *entry, *secIdx, *secNo;
    /*
    -1 file tidak ditemukan
    */

    banyakParent = parsePath(path, parentDirs, fileName);

    readSector(dir, 0x101);
    readSector(dir+SECTOR_SIZE, 0x102);
    readSector(sec, 0x103);

    i = 0;
    do
    {
        entry = dir+i;
        // *(entry+2) adalah posisi nama file pada entry
        success = strcmp(entry+2, fileName) == 0;
        i += ROW_SIZE;
    } while (i < 2*SECTOR_SIZE && !success);

    if (!success) // file tidak ditemukan
    {
        *result = -1;
        return;
    }

    i = 0;
    secIdx = entry+1;
    secNo = sec+((*secIdx)*ROW_SIZE);
    while (*(secNo+i) != 0 && i < ROW_SIZE)
    {
        readSector(buffer+(i*SECTOR_SIZE), *(secNo+i));
        i++;
    }
}