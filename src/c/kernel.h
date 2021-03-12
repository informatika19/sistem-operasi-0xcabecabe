/**
 * kernel.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 20 Februari 2021
 *
 * Header yang berisi fungsi-fungsi utama kernel
 */

#ifndef _KERNEL_CABECABE_H_
#define _KERNEL_CABECABE_H_

typedef char byte;

#define SECTOR_SIZE 512

#define VIDEO_SEGMENT   0xB000
#define OFFSET_CHAR     0x8000
#define OFFSET_COLOR    0x8001

// Defining colors
#define BLACK       0x0
#define BLUE        0x1
#define GREEN       0x2
#define CYAN        0x3
#define RED         0x4
#define MAGENTA     0x5
#define BROWN       0x6
#define LTGRAY      0x7
#define DRGRAY      0x8
#define LTBLUE      0x9
#define LTGREEN     0xA
#define LTCYAN      0xB
#define LTRED       0xC
#define LTMAGENTA   0xD
#define YELLOW      0xE
#define WHITE       0xF

/** *** Diimplementasikan di Assembly *** **/
/**
 * Fungsi untuk menulis sebuah karakter pada segment memori dengan offset
 * tertentu
 * @param segment segment memori yang ingin dituliskan karakter
 * @param address address ke memori
 * @param character karakter yang ingin dituliskan
 */
void putInMemory(int segment, int address, char character);
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
/**
 * Fungsi untuk menyiapkan interrupt vector untuk memanggil kode ketika
 * interrupt 0x21 dipanggil
 */
void makeInterrupt21();

/** *** Diimplementasikan di C *** **/
/**
 * Fungsi yang dipanggil ketika terjadi interrupt 0x21
 * @param AX
 * @param BX
 * @param CX
 * @param DX
 */
void handleInterrupt21(int AX, int BX, int CX, int DX);
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
 * Fungsi untuk mengosongkan buffer
 * @param buffer address ke buffer yang ingin dikosongkan
 * @param length panjang/banyak 0 yang mau diisi
 */
void clear(char *buffer, int length);
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
/**
 * Fungsi untuk membaca sector
 * @param buffer
 * @param sector
 */
void readSector(char *buffer, int sector);
/**
 * Fungsi untuk menulis ke sector
 * @param buffer
 * @param sector
 */
void writeSector(char *buffer, int sector);
/**
 * Fungsi untuk menuliskan file
 * @param buffer isi file yang ingin ditulis
 * @param path path ke file
 * @param sectors ukuran file
 * @param parentIndex index parent di sektor files
 */
void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
/**
 * Fungsi untuk membaca file
 * @param buffer buffer untuk menampung isi file
 * @param path path ke file
 * @param result hasil pembacaan file
 * @param parentIndex index parent di sektor files
 */
void readFile(char *buffer, char *path, int *result, char parentIndex);

#endif
