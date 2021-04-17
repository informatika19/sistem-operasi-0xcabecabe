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
 * Fungsi untuk menuliskan sebuah string ke layar.
 * @param str string yang ingin dituliskan
 */
void print(char *str);
/**
 * Fungsi untuk membaca sebuah string
 * @param buf buffer untuk "menampung" yang ingin dibaca
 */
char *read(char *buf);
/**
 * Fungsi untuk menuliskan integer menjadi char
 * @param number angka yang ingin dituliskan
 */
void printNumber(int number);

#endif
