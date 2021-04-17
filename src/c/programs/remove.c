#include "../lib.h"

int main(){
    int argc; char argv[5][20];

    char dir[2*SECTOR_SIZE], map[SECTOR_SIZE], sec[SECTOR_SIZE];
    char emptySec[512];

    char parentIndex;
    char filename[14];
    char temp[64][14];
    int i, j, test, pathSize;
    char secIndex;

    bool found = false;

    char *path = argv[1];
    char cwdIdx = atoi(argv[2])&0xFF;

    getSector(map, 0x100);
    getSector(dir, 0x101);
    getSector(dir + SECTOR_SIZE, 0x102);
    getSector(sec, 0x103);

    fillBuffer(emptySec, 512, 0);

    if (*path != '\0') {
        test = getFileIndex(path, cwdIdx, dir);

        if (test == -1) {
            print(path);
            print(" tidak ada.\n");
            return -1;
        }

        parentIndex = test & 0xFF;
        pathSize = strntoken(path, temp, '/', 14);
        strncpy(filename, temp[pathSize - 1], 14);
        if (*(dir + parentIndex + 1) != '\xFF') {
            print(path);
            print(" bukan direktori.\n");
            return -1;
        }
    }

    i = 0;
    while (i < 2*SECTOR_SIZE && found == false) {
        if (*(dir+i)== parentIndex && strncmp(dir + i + 2, filename, 14)){
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
            updateSector(emptySec,(*sec + secIndex*16 + j));
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
        print("File tidak ada di dalam directory\n");
    }

}
