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
 * @param *string string yang ingin dituliskan
 */
void printString(char *string);
/**
 * Fungsi untuk membaca string dari keyboard
 * @param *string string yang akan menampung bacaan dari keyboard
 */
void readString(char *string);
/**
 * Fungsi untuk mengosongkan buffer
 * @param buffer address ke buffer yang ingin dikosongkan
 * @param length panjang/banyak 0 yang mau diisi
 */
void clear(char *buffer, int length);

int main()
{
    /*makeInterrupt21();*/
    putInMemory(VIDEO_SEGMENT, OFFSET_CHAR, 'H');
    putInMemory(VIDEO_SEGMENT, OFFSET_COLOR, 0xD);
    putInMemory(VIDEO_SEGMENT, OFFSET_CHAR+2, 'A');
    putInMemory(VIDEO_SEGMENT, OFFSET_COLOR+2, 0x2);
    putInMemory(VIDEO_SEGMENT, OFFSET_CHAR+4, 'I');
    putInMemory(VIDEO_SEGMENT, OFFSET_COLOR+4, 0x1);

    while(1);
}

void handleInterrupt21(int AX, int BX, int CX, int DX)
{
    /*switch(AX)*/
    /*{*/
        /*case 0x0:*/
            /*printString(BX);*/
            /*break;*/
        /*case 0x1:*/
            /*printString(BX);*/
            /*break;*/
        /*default:*/
            /*printString("Invalid interrupt");*/
    /*}*/
}
