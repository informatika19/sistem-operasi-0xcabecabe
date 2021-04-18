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
 * Fungsi untuk mendapatkan sektor tertentu
 * @param buf untuk menampung sektor yang "diambil"
 * @param secNo nomor sektor yang ingin diambil
 * @return buf
 */
char *getSector(char *buf, int secNo);
/**
 * Fungsi untuk memperbarui sektor tertentu
 * @param isi baru dari sektor
 * @param secNo nomor sektor yang ingin diambil
 */
void updateSector(char *buf, int secNo);
/**
 * Fungsi untuk membaca file. Penjelasan nilai result setelah eksekusi:
 * -1: path invalid
 * @param buffer buffer untuk menampung isi file
 * @param path nama file yang ingin dibaca
 * @param result hasil pembacaan file (banyak block yang dibaca)
 * @param parentIndex index dari cwd
 * @return buffer
 */
char *getFile(char *buffer, char *path, int *result, char parentIndex);
/**
 * Fungsi untuk menulis file. Penjelasan untuk nilai sectors setelah eksekusi:
 * -1: file sudah ada
 * -2 atau -3: tidak cukup space untuk menuliskan file
 * -4: path invalid
 * @param buffer isi file yang ingin ditulis
 * @param path nama file yang ingin ditulis
 * @param sectors banyak block file
 * @param parentIndex index dari cwd
 * @return sectors
 */
int updateFile(char *buffer, char *path, int *sectors, char parentIndex);
/**
 * Fungsi untuk menghapus file dari filesystem. BELUM BISA MENGHAPUS FOLDER.
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

#endif
