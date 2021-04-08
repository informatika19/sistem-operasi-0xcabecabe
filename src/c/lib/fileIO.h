/**
 * fileIO.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 13 Maret 2021
 *
 * Library untuk operasi pada file
 */

#ifndef _FILEIO_CABECABE_H_
#define _FILEIO_CABECABE_H_

#include "boolean.h"
#define KERNEL_SIZE 10
#define SECTOR_SIZE 512

/**
 * Fungsi untuk menuliskan isi file ke filesystem
 * @param buffer isi file yang ingin ditulis
 * @param path nama file yang ingin ditulis
 * @param sectors ukuran file
 * @param parentIndex index parent dari file
 */
void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
/**
 * Fungsi untuk membaca isi file di filesystem
 * @param buffer buffer untuk menampung isi file
 * @param path nama file yang ingin dibaca
 * @param result hasil pembacaan file (banyak buffer yang dihasilkan)
 * @param parentIndex index parent dari file
 */
void readFile(char *buffer, char *path, int *result, char parentIndex);
/**
 * Fungsi untuk mendapatkan nomor index/nomor entri dari sektor files untuk
 * suatu file dengan fileName dan parentIndex tertentu
 * @param char *fileName nama dari file yang akan dicek
 * @param char parentIndex indeks dari parent  dicek
 * @return nomor index/nomor entri file yang dicari pada sektor files
 */
int getFileIndex(char *path, char parentIndex, char *dir);
/**
 * Fungsi untuk membaca sector
 * @param buffer
 * @param sector
 */
void readSector(char *buffer, int sector);
/**
 * Fungsi untuk menulis ke sector
 * @param buffer
 * @param sector
 */
void writeSector(char *buffer, int sector);

#endif
