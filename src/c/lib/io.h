/**
 * io.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 13 Maret 2021
 *
 * Library untuk input/output string, grafik, angka
 */

#ifndef _IO_CABECABE_H_
#define _IO_CABECABE_H_

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
/*
 * Fungsi untuk menuliskan integer menjadi char
 * @param number angka yang ingin dituliskan
 */
void printNumber(int number);
/**
 * Fungsi untuk mengosongkan buffer
 * @param buffer address ke buffer yang ingin dikosongkan
 * @param length panjang/banyak 0 yang mau diisi
 */
void clear(char *buffer, int length);
/**
 * Fungsi untuk mengeksekusi sebuah program
 * @param fileName nama file
 * @param segment nomor sektor program pada image file OS
 * @param success hasil eksekusi
 * @param parentIndex index cwd
 */
void executeProgram(char *fileName, int segment, int *success,
                    char parentIndex);

#endif
