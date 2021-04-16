#include "../lib.h"

int main(){
    int argc; char argv[5][20];

    char dir[2*SECTOR_SIZE], map[SECTOR_SIZE], sec[SECTOR_SIZE];
    char emptySec[512];

    char parentIndex;
    char *filename;
    char *temp;
    int i, j, test, pathSize;
    char secIndex;

    bool found = false;

    char *path = argv[1];
    char cwdIdx = atoi(argv[2])&0xFF;

    readSector(map, 0x100);
    readSector(dir, 0x101);
    readSector(dir + SECTOR_SIZE, 0x102);
    readSector(sec, 0x103);

    clear(emptySec, 512);

    if (path != '\0') {
        test = getFileIndex(path, parentIndex, dir);

        if (test == -1) {
            printString(path);
            printString(" tidak ada.\n");
            return -1;
        }

        parentIndex = test & 0xFF;
        pathSize = strntoken(path,temp, '/', 14);
        filename = temp[pathSize - 1];
        if (*(dir + parentIndex + 1) != '\xFF') {
            printString(path);
            printString(" bukan direktori.\n");
            return -1;
        }
    }

    i = 0;
    while (i < 2*SECTOR_SIZE && found == false) {
        if (*(dir+i)== parentIndex && *(dir+i+2) == filename){
            secIndex = *dir + i + 1;
            found = true;
        }
        else {
            i+=16;
        }
    }

    if (found){
        //cari sector sector milik file
        j = 0;
        //hapus buffer dan file sector
        while(*(sec + secIndex*16 + j) != 0 && j < 16){
            writeSector(emptySec,(*sec + secIndex*16 + j));
            *(map + *(sec + secIndex*16 + j)) = 0;
            *(sec+secIndex*16+j) = 0;
            j++;
        }
        //hapus sector dir nya
        j = 0;
        while(*(dir + i + j) != 0 && j < 16){
            *(dir+i+j) = 0;
            j++;
        }
    }

    else{
        printString("File tidak ada di dalam directory\n");
    }

}