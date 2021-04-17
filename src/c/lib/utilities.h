/**
 * utilities.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 18 April 2021
 *
 * Library untuk fungsi utility
 */

#ifndef _UTILITIES_CABECABE_H_
#define _UTILITIES_CABECABE_H_
/**
 * Fungsi untuk mengisi buffer/memori dengan sebuah filler berukuran 1 byte
 * (char)
 * @param buf buffer yang ingin diisi
 * @param n banyak yang mau diisi
 * @param filler karakter untuk mengisi
 * @return buf
 */
char *fillBuffer(char *buf, int n, char filler);
/**
 * Fungsi untuk mengeksekusi sebuah program
 * @param fileName nama file
 * @param segment nomor sektor program pada image file OS
 * @param success hasil eksekusi
 * @param parentIndex index cwd
 */
void exec(char *fileName, int segment, int *success, char parentIndex);
#endif
