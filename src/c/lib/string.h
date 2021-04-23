/**
 * string.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 6 Maret 2021
 *
 * Library yang berisi fungsi-fungsi utilitas pada string
 */

#ifndef _STRING_CABECABE_H_
#define _STRING_CABECABE_H_

/**
 * Fungsi untuk menghitung panjang null-terminated string (excluding bagian \0)
 * @param *s null-terminated string yang ingin dihitung panjangnya
 * @return panjang string s excluding null terminator (\0)-nya
 */
int strlen(char *s);

/**
 * Fungsi untuk membandingkan panjang dua buah string. Pemeriksaan akan
 * berhenti sampai salah satu string mencapai '\0'
 * @param *s1 null-terminated string pertama
 * @param *s2 null-terminated string kedua
 * @return mengembalikan:
 * - 0 jika kedua string sama,
 * - negatif jika s1 lebih
 * - pendek, positif jika s1 lebih panjang
 */
int strcmp(char *s1, char *s2);

/**
 * Fungsi untuk membandingkan panjang dua buah string. Pemeriksaan akan
 * berhenti sampai salah satu string mencapai '\0' atau sudah dibandingkan n
 * karakter
 * @param *s1 null-terminated string pertama
 * @param *s2 null-terminated string kedua
 * @param n banyak karakter yang ingin dibandingkan
 * @return mengembalikan:
 * - 0 jika kedua string sama,
 * - negatif jika s1 lebih
 * - pendek, positif jika s1 lebih panjang
 */
int strncmp(char *s1, char *s2, int n);

/**
 * Fungsi untuk menyalin string
 * @param dest tempat untuk menampung string salinan
 * @param src string yang akan disalin
 * @return string di dest
 */
char *strcpy(char *dest, char *src);

/**
 * Fungsi untuk menyalin string sebanyak n karakter
 * @param dest tempat untuk menampung string salinan
 * @param src string yang akan disalin
 * @param n banyak karakter string yang ingin disalin
 * @return string di dest
 */
char *strncpy(char *dest, char *src, int n);

/**
 * Menggabungkan dua string, string hasil akan dikembalikan atau di-append
 * ke argumen pertama
 * @param dest string "original" (akan digabungkan dengan src)
 * @param src string yang ingin di-append
 * @param n banyak string yang ingin digabungkan
 * @return string di dest
 */
char *strcat(char *dest, char *src);

/**
 * Menggabungkan dua string, string hasil akan dikembalikan atau di-append
 * ke argumen pertama
 * @param dest string "original" (akan digabungkan dengan src). Ukuran dest
 * minimal strln(dest)+n+1
 * @param src string yang ingin di-append
 * @param n banyak string yang ingin digabungkan
 * @return string di dest
 */
char *strncat(char *dest, char *src, int n);

/**
 * tl;dr: mirip dengan strtok atau split string di JS atau Python.
 * Tokenize/memisahkan string berdasarkan sebuah delim. Hasilnya adalah sebuah
 * array of strings pada dest. Akan mengembalikan banyak token pada dest. Jika
 * karakter beberapa pertama atau karakter terakhir hasil adalah delim, akan
 * di-remove
 * @param src string yang ingin di-tokenize
 * @param dest array of strings berisihasil tokenizing string
 * @param delim delimiter/pemisah antar token
 * @param n panjang maksimum setiap token + 1
 * @return banyak token pada array of strings
 */
int strntoken(char *src, char *dest, char delim, int n);

/**
 * Fungsi untuk convert string menjadi int
 * @param str string yang ingin diubah menjadi int
 */
int atoi(char *str);

/**
 * Konversi dari int menjadi string
 * @param buf tempat menampung string
 * @param i angka yang mau diubah
 * @param n ukuran buf
 * @return buf
 */
char *itoa(char *buf, int i, int n);

#endif
