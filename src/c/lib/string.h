/**
 * string.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 6 Maret 2021
 *
 * Header yang berisi fungsi-fungsi untuk operasi pada string
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

#endif
