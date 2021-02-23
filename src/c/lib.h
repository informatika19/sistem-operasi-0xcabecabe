/**
 * lib.h
 * Alvin W., Josep M., Rehagana C.K.
 * 20 Februari 2020
 *
 * Header yang berisi fungsi-fungsi tambahan untuk kernel
 */
#ifndef _LIB_KERNEL_CABECABE_H_
#define _LIB_KERNEL_CABECABE_H_

/** *** Diimplementasikan di Assembly *** **/
/**
 * Fungsi untuk mendapatkan posisi cursor di layar
 */
int getCursor();
/**
 * Fungsi untuk load binary file dengan nama yang sudah ditentukan program
 */
int *loadBin();

/** *** Diimplementasikan di C *** **/
/**
 * Fungsi untuk menghitung modulo aritmatika dari a dan b.
 * a % b
 * @param a
 * @param b
 */
int mod(int a, int b);
#endif
