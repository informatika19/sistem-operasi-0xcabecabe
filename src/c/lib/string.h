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
#endif
