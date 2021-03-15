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

int getFileIndex(char *path, char parentIndex)
{
    char dir[2*SECTOR_SIZE];
    char parent;
    int i, j, banyakParent;
    char *nama, *fileName, parents[64][14];
    bool isfound = true;

    readSector(dir,0x101);
    readSector(dir+SECTOR_SIZE, 0x102);


    banyakParent = parsePath(path, parents, fileName);

    // bandingin nama dgn yg ada di path
    // misalkan /usr/share/include/lib/asd
    // bandingin "asd" dengan nama file yg lagi diperiksa

    // kalo udah, if (isPathValid("/usr/share/include/lib"))
    // return true else repeat loop

    // perlu ngecek path di sector dari root?
    // misalnya 02 00 74 65 73 74 00 ... 0x00
    //          FF FF 74 65 73 74 00 ... 0x10
    //          01 00 74 66 66 66 66 ... 0x10*2 dir+0x10*2
    // simpen indeks terbesarnya berarti 01
    // ngeceknya mundur berdasarkan indeks itu
    // path = "/test/tffff/test"
    // parents = ["/", "test", "tffff"]
    // misalnya ./text.txt ini bener ga?
    // text.txt dicek sama yang di dir yang indeksnya 01
    // cara ngakses dir nya gmn? (ngakses 74 65 73 ..)

    // cek dlu index valid atau nggak:
    // test = strncmp((dir+(parentIndex*0x10))+2, parents[banyakParent-1], 14) == 0
    // test = test ? isPathValid(...) : false;
    // return test;

    j = 1; // untuk mundur di string path
    nama = parents[banyakParent-1];
    parentIndex++;
    do
    {
        // path paling ujung kanan
        if (!isIndexValid(nama, parentIndex)) {
            isfound = false;
        }

        nama = parents[banyakParent-j];
        j++;
        parentIndex--;
    } while (i < 2*SECTOR_SIZE && isfound && banyakParent-j==0);

    return isfound;
}

/**
 * Fungsi untuk mengecek apakah kombinasi filename dan parentindex terdapat di
 * directory
 * @param char *fileName nama dari file yang akan dicek
 * @param char parentIndex indeks dari parent  dicek
 * @return bool a
 */
bool isIndexValid(char *fileName, char parentIndex) {
    char dir[2*SECTOR_SIZE];
    bool found = false;
    char *entry;
    int i = 0;
    readSector(dir,0x101);
    readSector(dir+SECTOR_SIZE,0x102);

    do
    {
        entry = dir+i;
        found = *entry == parentIndex && strncmp(entry+2, fileName, 14);
        i += 0x10;
    } while (i < 2*SECTOR_SIZE && !found);
    return found;
}

int parsePath(char *path, char *parents, char *fname)
{
    int i, j, n;
    char cur[14];
    bool isParentDone = false;

    i = 0, j = 0;
    if (*path == '/')
    {
        *(parents+j+i) = '/';
        *(parents+j+(i+1)) = '\0';
        j += 14;
        path++;
    }
    while (*path != '\0')
    {
        switch (*path)
        {
            case '/':
                *(parents+j+i) = 0;
                j += 14 * (i != 0);
                i = 0;
                isParentDone = true;
                break;
            default:
                *(parents+j+i) = *path;
                i++;
                isParentDone = false;
        }

        if (isParentDone) // ngecek ada '.' atau '..' atau ngga
        {
            n = 14 * (strncmp(parents+j-14, ".", 14) == 0) +
                28 * (strncmp(parents+j-14, "..", 14) == 0);
            j -= n;
            clear(parents+j, n);
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
    int i, j, entry, sectorNeeded, sectorUsed, sectorFree,
        sectorsToUse[16], entrySectors;
    bool alreadyExists = false, parentExists = (parentIndex == 0xFF);
    char map[SECTOR_SIZE],
         dir[2*SECTOR_SIZE],
         sec[SECTOR_SIZE];
    char fileName[14], parentDirs[64][14]; // array of strings untuk parent dari path
    /*
    -1 file sudah ada
    -2 tidak cukup entri di sektor files
    -3 tidak cukup sektor kosong
    -4 folder tidak valid
    */

    if (*sectors > 16) { *sectors = -3; return; }

    parsePath(path, parentDirs, fileName);

    sectorNeeded = *sectors;
    readSector(map, 0x100);
    readSector(dir, 0x101);
    readSector(dir+SECTOR_SIZE, 0x102);
    readSector(sec, 0x103);

    // ngecek file dengan yang sama di parent index yang sama udah ada atau belum
    i = 0;
    while (i < 2*SECTOR_SIZE && !alreadyExists)
    {
        parentExists = !parentExists ? *(dir+i) == parentIndex : parentExists;
        alreadyExists = *(dir+i) == parentIndex
                        && strncmp(dir+i+2, fileName, 13) == 0;
        i += 0x10;
    }
    if (alreadyExists) { *sectors = -1; return; }
    if (!parentExists) { *sectors = -4; return; }

    // nyari entri kosong di sektor files yang kosong kosong
    entry = 0;
    for (; *(dir+entry+2) && entry < 2*SECTOR_SIZE; entry += 0x10);
    if (entry >= 2*SECTOR_SIZE) { *sectors = -2; return; }

    // nyari sektor-sektor di map yang kosong untuk diisi file
    // sektor yang digunakan bisa tidak kontigu
    i = 0;
    j = 0;
    while (i < SECTOR_SIZE)
    {
        if (*(map+i) == 0x00)
        {
            sectorFree++;
            if (j < 16) sectorsToUse[j++] = i;
        }
        ++i;
    }
    // TODO: Bug aneh di sini
    if (sectorFree < sectorNeeded) { *sectors = -3; return; }

    // isi sektornya
    i = 0;
    while (i < sectorNeeded)
    {
        writeSector(buffer+(i*SECTOR_SIZE), sectorsToUse[i]);
        *(map + sectorsToUse[i]) = 0xFF;
        ++i;
    }

    // isi sektor sectors (0x103)
    // cari entri di sektor sectors yang kosong
    entrySectors = 0;
    for (;*(sec+(entrySectors)) && entrySectors < SECTOR_SIZE
         ;entrySectors += 0x10);

    *sectors = 0;
    for (i = 0; i < sectorNeeded; ++i, ++(*sectors))
        *(sec+entrySectors+i) = sectorsToUse[i];

    // akusisi entry yang ditemukan sebelumnya
    *(dir+entry) = parentIndex;
    *(dir+entry+1) = entrySectors/0x10;
    strncpy(dir+entry+2, fileName, 14);

    // tulis perubahan
    writeSector(map, 0x100);
    writeSector(dir, 0x101);
    writeSector(dir+512, 0x102);
    writeSector(sec, 0x103);
}

void readFile(char *buffer, char *path, int *result, char parentIndex)
{
    int i;
    bool success;
    char dir[2*SECTOR_SIZE], sec[SECTOR_SIZE];
    char fileName[14];//, parentDirs[64][14]; // array of strings untuk parent dari path
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
        success = *entry == parentIndex && strncmp(entry+2, fileName, 13) == 0;
        i += 0x10;
    } while (i < 2*SECTOR_SIZE && !success);
    // TODO: kelemahan /bin/sh ekivalen dengan /asdsad/fds/gg/sd/a/a/d/ea/bin/sh

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
