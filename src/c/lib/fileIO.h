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
 * @param sectors banyak block file
 * @param parentIndex index dari cwd
 */
void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
/**
 * Fungsi untuk membaca isi file di filesystem
 * @param buffer buffer untuk menampung isi file
 * @param path nama file yang ingin dibaca
 * @param result hasil pembacaan file (banyak block yang dibaca)
 * @param parentIndex index dari cwd
 */
void readFile(char *buffer, char *path, int *result, char parentIndex);
/**
 * Fungsi untuk menghapus file dari filesystem
 * @param path path ke file yang ingin di hapus
 * @param result hasil penghapusan file (banyak block yang dihapus)
 * @param parentIndex index dari cwd
 */
void removeFile (char *path, int *result, char parentIndex);
/**
 * Fungsi untuk mendapatkan nomor index/nomor entri dari sektor files untuk
 * suatu file dengan fileName dan parentIndex tertentu
 * @param char *fileName nama dari file yang akan dicek
 * @param char parentIndex indeks dari parent yang akan dicek
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
