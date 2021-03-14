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
        switch (*path)
        {
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
    int i, filesIdx, writeSectorCount;
    char map[SECTOR_SIZE],
         dir[2*SECTOR_SIZE],
         sec[SECTOR_SIZE],
         useSectors[16];
    char *fileName, *parentDirs[64][14]; // array of strings untuk parent dari path
    /*
    -1 file sudah ada
    -2 tidak cukup entri di files
    -3 tidak cukup sektor kosong
    -4 folder tidak valid
    */

    parsePath(path, parentDirs, fileName);

    writeSectorCount = *sectors;
    readSector(map, 0x100);
    readSector(dir, 0x101);
    readSector(dir+SECTOR_SIZE, 0x102);
    readSector(sec, 0x103);

    // nyari index di sektor files yang kosong kosong
    // ini harus cek parent directories
    filesIdx = 0;
    while (dir[filesIdx] && filesIdx < 2*SECTOR_SIZE)
        filesIdx += 0x10;
    if (filesIdx >= 2*SECTOR_SIZE)
    {
        *sectors = -2;
        return;
    }
    clear(dir+filesIdx, 0xF); // nulis nama file
    strncpy(dir+filesIdx+2, fileName, 14);

    // nyari index di sektor

    // nyari sektor-sektor di map yang kosong untuk diisi file
    // sektor di byte ke-1 sampai ke-10 (indeks 0--9) dipakai untuk kernel
    // indexing untuk map dimulai dari KERNEL_SIZE
    i = KERNEL_SIZE;
    /** cek jumlah sektor di map cukup untuk buffer file **/

    // ini di akhir
    *sectors = 0;
}

void readFile(char *buffer, char *path, int *result, char parentIndex)
{
    int i;
    bool success;
    char dir[2*SECTOR_SIZE], sec[SECTOR_SIZE];
    char *fileName;//, *parentDirs[64][14]; // array of strings untuk parent dari path
    char *entry, secIdx, *secNo;
    /*
    -1 file tidak ditemukan
    */

    parsePath(path, 0, fileName);

    readSector(dir, 0x101);
    readSector(dir+SECTOR_SIZE, 0x102);
    readSector(sec, 0x103);

    i = 0;
    do
    {
        entry = dir+i;
        // *(entry+2) adalah posisi nama file pada entry
        success = *entry == parentIndex && strcmp(entry+2, fileName) == 0;
        i += 0x10;
    } while (i < 2*SECTOR_SIZE && !success);

    if (!success) // file tidak ditemukan di parent atau parent tidak ada
    {
        *result = -1;
        return;
    }

    i = 0;
    secIdx = *(entry+1);
    secNo = sec+(secIdx*0x10);
    while (*secNo && i < 0x10)
    {
        readSector(buffer+(i*SECTOR_SIZE), *secNo);
        i++;
        secNo++;
    }

    *(buffer+(i*SECTOR_SIZE)) = 0;
    *result = i;
}
