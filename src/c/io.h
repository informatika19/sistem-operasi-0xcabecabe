/**
 * io.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 13 Maret 2021
 *
 * Header yang berisi fungsi-fungsi pada kernel yang berhubungan dengan input
 * dan output
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
/**
 * Fungsi untuk menuliskan logo ASCII
 */
void printLogoASCII();
/**
 * Fungsi untuk menuliskan logo grafik
 */
void printLogoGrafik(int sisi);

#endif
