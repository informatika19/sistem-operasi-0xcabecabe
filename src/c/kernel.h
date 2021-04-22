/**
 * kernel.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 20 Februari 2021
 *
 * Header yang berisi untuk fungsi-fungsi "utama" kernel
 */

#ifndef _KERNEL_CABECABE_H_
#define _KERNEL_CABECABE_H_

#include "lib/lib_asm.h"

#define VIDEO_SEGMENT 0xB000
#define OFFSET_CHAR 0x8000
#define OFFSET_COLOR 0x8001

/* *** Diimplementasikan di Assembly *** */
/**
 * Fungsi untuk menyiapkan interrupt vector untuk memanggil kode ketika
 * interrupt 0x21 dipanggil
 */
void makeInterrupt21();
/**
 * Fungsi untuk menaruh sebuah byte di memori. Perhitungan adalah:
 * segment * (2 ** 4) + address
 * @param segment segment memori yang ingin dituliskan karakter
 * @param address address ke memori
 * @param character karakter yang ingin dituliskan
 */
void putInMemory(int segment, int address, char character);
/**
 * Fungsi untuk me-launch sebuah program yang sudah di-load ke memori.
 * @param segment segmen di memori
 */
void launchProgram(int segment);

/* *** Diimplementasikan di C *** */
/**
 * Fungsi yang dipanggil ketika terjadi interrupt 0x21
 * @param AX
 * @param BX
 * @param CX
 * @param DX
 */
void handleInterrupt21(int AX, int BX, int CX, int DX);
/**
 * Fungsi untuk menuliskan logo grafik berbentuk persegi
 * @param sisi banyak pixel/panjang sisi pada logo
 */
void printLogoGrafik(int sisi);
/**
 * Fungsi untuk mengeksekusi sebuah program
 * @param fileName nama file
 * @param segment nomor sektor program pada image file OS
 * @param success hasil eksekusi
 * @param parentIndex index cwd
 */
void executeProgram(char *fileName, int segment, int *success,
                    char parentIndex);
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
 * Fungsi menuliskan string ke output
 * @param string string yang ingin dituliskan
 */
void printString(char *string);
/**
 * Fungsi untuk membaca string dari keyboard
 * Possible buffer overflow >:((((
 * @param string string yang akan menampung bacaan dari keyboard
 */
void readString(char *string);
/**
 * Fungsi untuk membuat persegi berkukuran sisi x sisi
 * @param sisi panjang sisi persegi
 * @param x absis untuk bagian kiri persegi
 * @param y ordinat untuk bagian atas persegi
 */
void bikinPersegi(int sisi, int warna, int x, int y);
/**
 * Fungsi untuk mengosongkan buffer
 * @param buffer address ke buffer yang ingin dikosongkan
 * @param length panjang/banyak 0 yang mau diisi
 */
void clear(char *buffer, int length);
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
