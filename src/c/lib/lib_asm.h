/**
 * lib.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 20 Februari 2021
 *
 * Header yang berisi fungsi-fungsi tambahan yang diimplementasikan di lib.asm
 */
#ifndef _ASM_CABECABE_H_
#define _ASM_CABECABE_H_

// Defining colors
#define BLACK 0x0
#define BLUE 0x1
#define GREEN 0x2
#define CYAN 0x3
#define RED 0x4
#define MAGENTA 0x5
#define BROWN 0x6
#define LTGRAY 0x7
#define DRGRAY 0x8
#define LTBLUE 0x9
#define LTGREEN 0xA
#define LTCYAN 0xB
#define LTRED 0xC
#define LTMAGENTA 0xD
#define YELLOW 0xE
#define WHITE 0xF

/**
 * Fungsi untuk mendapatkan posisi baris cursor di layar
 */
int getCursorRow();
/**
 * Fungsi untuk mendapatkan posisi kolom cursor di layar
 */
int getCursorCol();
/**
 * Fungsi untuk memanggil interrupt dengan nomor tertentu dan meneruskan
 * parameter AX, BX, CX, DX
 * @param number angka interrupt
 * @param AX
 * @param BX
 * @param CX
 * @param DX
 */
int interrupt(int number, int AX, int BX, int CX, int DX);

#endif
