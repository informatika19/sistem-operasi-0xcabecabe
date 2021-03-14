/**
 * filesystem.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 13 Maret 2021
 *
 * Header yang berisi fungsi-fungsi untuk penunjang filesystem dan file
 * operation pada kernel
 */

#ifndef _FILESYSTEM_CABECABE_H_
#define _FILESYSTEM_CABECABE_H_

#define KERNEL_SIZE 10

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
/**
 * Fungsi untuk menuliskan isi file ke filesystem
 * @param buffer isi file yang ingin ditulis
 * @param path path ke file
 * @param sectors ukuran file
 * @param parentIndex index parent di sektor files
 */
void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
/**
 * Fungsi untuk membaca isi file di filesystem
 * @param buffer buffer untuk menampung isi file
 * @param path path ke file
 * @param result hasil pembacaan file
 * @param parentIndex index parent di sektor files
 */
void readFile(char *buffer, char *path, int *result, char parentIndex);
/**
 * Fungsi untuk parsing path
 * @param path path yang ingin di-parse
 * @param parents array of char pointer, setiap elemen punya 14 byte, berisi
 * nama direktori parent. Array ini menampung 64*14 bytes.
 * @param fname nama file/direktori terakhir pada path
 * @return banyak direktori parent
 */
int parsePath(char *path, char *parents, char *fname);

#endif
