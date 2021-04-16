/**
 * kernel.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 20 Februari 2021
 *
 * Header yang berisi untuk fungsi-fungsi "utama" kernel
 */

#ifndef _KERNEL_CABECABE_H_
#define _KERNEL_CABECABE_H_

typedef char byte;

#include "lib_asm.h"

#define VIDEO_SEGMENT 0xB000
#define OFFSET_CHAR 0x8000
#define OFFSET_COLOR 0x8001

/* *** Diimplementasikan di Assembly *** */
/**
 * Fungsi untuk menulis sebuah karakter pada segment memori dengan offset
 * tertentu
 * @param segment segment memori yang ingin dituliskan karakter
 * @param address address ke memori
 * @param character karakter yang ingin dituliskan
 */
void putInMemory(int segment, int address, char character);
/**
 * Fungsi untuk menyiapkan interrupt vector untuk memanggil kode ketika
 * interrupt 0x21 dipanggil
 */
void makeInterrupt21();

/* *** Diimplementasikan di C *** */
/**
 * Fungsi yang dipanggil ketika terjadi interrupt 0x21
 * @param AX
 * @param BX
 * @param CX
 * @param DX
 */
void handleInterrupt21(int AX, int BX, int CX, int DX);
/**
 * Fungsi untuk menuliskan logo grafik berbentuk persegi
 * @param sisi banyak pixel/panjang sisi pada logo
 */
void printLogoGrafik(int sisi);
/**
 * Fungsi untuk mengeksekusi sebuah program
 * @param fileName nama file
 * @param segment tempat menaruh program di memori
 * @param success hasil eksekusi
 * @param parentIndex index cwd
 */
void executeProgram(char *fileName, int segment, int *success,
                    char parentIndex);

#endif
