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
/**
 * Fungsi untuk mengirimkan argumen untuk suatu program. Current working
 * directory index akan dianggap sebagai argumen pertama (indeks 0).
 * @param argv string berisi argumen yang mau dikirim (dipisahkan spasi)
 * @param cwdIndex current working directory index
 * @return 0 jika berhasil, -1 jika gagal
 */
int sendArguments(char *argv, char cwdIndex);
/**
 * Fungsi untuk menerima argumen yang dikirimkan. Panjang string maksimum
 * setiap argumen yang dibaca adalah 19 karakter.
 * @param argv array of string untuk menampung argumen-argumen
 * @return banyak argumen yang dibaca, jika gagal akan return -1
 */
int getArguments(char *argv);
#endif
