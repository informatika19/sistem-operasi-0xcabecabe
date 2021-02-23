/**
 * lib.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 20 Februari 2021
 *
 * Header yang berisi fungsi-fungsi tambahan untuk kernel
 */
#ifndef _LIB_KERNEL_CABECABE_H_
#define _LIB_KERNEL_CABECABE_H_

/** *** Diimplementasikan di Assembly *** **/
/**
 * Fungsi untuk mendapatkan posisi baris cursor di layar
 */
int getCursorRow();
/**
 * Fungsi untuk mendapatkan posisi kolom cursor di layar
 */
int getCursorCol();
/**
 * Fungsi untuk load binary file dengan nama yang sudah ditentukan program
 */
int *loadBin();
/**
 * Fungsi untuk menuliskan logo
 */
void printLogo();

/** *** Diimplementasikan di C *** **/
/**
 * Fungsi untuk menghitung modulo aritmatika dari a dan b.
 * a % b
 * @param a
 * @param b
 * @returns a mod b
 */
int mod(int a, int b);
#endif
