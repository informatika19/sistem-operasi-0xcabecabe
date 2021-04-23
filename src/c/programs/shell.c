/**
 * shell.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 15 Maret 2021
 *
 * Implementasi program shell untuk OS 0xCABECABE
 */

#include "shell.h"

#include "../lib.h"

int main() {
    int secSize = 1;
    char command[10 * 20];  // kalo pointer doang takut error
    char argv[9][20];
    char *argvStart;

    char hist[HIST_SIZE][10 * 20];

    char cwdName[14], prompt[27];
    char cwdIdxStr[3];

    char cwdIdx;
    int argc;
    int histc = 0;
    int i;
    int execRes = 0;

    cwdIdx = 0xFF;

    fillBuffer(prompt, 27, 0);
    strncpy(prompt, "0xCABECABE", 10);
    strncat(prompt, "@", 1);

    while (true) {
        // baca cwdIdx
        argc = getArguments(argv);
        if (argc < 0) {
            print("Error starting shell.\n");
            while (true) // udah ngehang aja di sini
                ;
        }
        cwdIdx = atoi(argv[0]) & 0xFF;

        // baca nama path
        getCwdName(cwdName, cwdIdx);

        // set prompt
        fillBuffer(prompt + 11, 16, 0);
        strncat(prompt, cwdName, strlen(cwdName));
        strncat(prompt, "> ", 2);

        do {
            fillBuffer(command, 200, 0);
            print(prompt);
            read(command);
        } while (*command == 0);

        // parse command
        argvStart = command;

        // cari tempat mulai nulis argumen, mau ada ataupun tidak
        while (*argvStart != ' ' && *argvStart != 0) {
            argvStart += 1 + (1 * (*argvStart == '\\'));
        }

        // bikin argv.tmp lagi, pertama isi ke argvTmp (buffer buat file argv.tmp)
        execRes = sendArguments(argvStart + 1, cwdIdx);
        if (execRes != 0) {
            print("Kesalahan ketika memahami perintah ");
            print(command);
            print(".\n");
            continue;
        }

        argc = strntoken(command, argv, ' ', 20);

        // cut-off command
        *argvStart = 0;

        // eksekusi perintah
        if (strncmp("cd", argv[0], 20) == 0) {
            if (argc != 2) {
                print("Penggunaan: cd <path/ke/direktori>\n");
            } else {
                cd(&cwdIdx, argv[1], cwdName);
            }
        } else if (strncmp("ls", argv[0], 20) == 0) {
            if (argc == 1) {
                listDir("", cwdIdx);
            } else if (argc == 2) {
                listDir(argv[1], cwdIdx);
            } else {
                print("Penggunaan: ls [path/ke/direktori]");
            }
        } else if (strncmp("cwd", argv[0], 20) == 0) {
            printNumber(cwdIdx);
            print(" - ");
            print(cwdName);
            print("\n");
        } else {
            exec(command, 0x2000, &execRes, 0x00);
            print("Perintah ");
            print(command);
            print(" tidak dikenali.\n");
        }

        // HISTORY
        histc = (histc >= HIST_SIZE) ? 0 : histc;
        for (i = 1; i < HIST_SIZE; i++) {
            strcpy(hist[i - 1], hist[i]);
        }
        strcpy(hist[HIST_SIZE - 1], command);
        histc++;
    }
}

void cd(char *parentIndex, char *path, char *newCwdName) {
    char dir[2 * SECTOR_SIZE];
    int tmpPI = *parentIndex, test;
    bool found = false, isDir = true;

    if (strncmp(path, ".", 20)) {
        if (strncmp(path, "/", 20) != 0) {
            getSector(dir, 0x101);
            getSector(dir + SECTOR_SIZE, 0x102);

            test = getFileIndex(path, *parentIndex, dir);
            tmpPI = test & 0xFF;
            found = test > -1;
        } else {
            found = true;
            tmpPI = 0xFF;
        }

        if (found) {
            isDir = *(dir + (tmpPI * 0x10) + 1) == '\xFF';
            test = 1;
            if (isDir || tmpPI == 0xFF) {  // cd ke root
                sendArguments("", tmpPI);
            } else {
                print(path);
                print(" bukan direktori.\n");
            }
        } else {
            print("Direktori ");
            print(path);
            print(" tidak ditemukan.\n");
        }
    }
    return;
}

void listDir(char *path, char parentIndex) {
    char dir[2 * SECTOR_SIZE];
    int i, test;

    getSector(dir, 0x101);
    getSector(dir + SECTOR_SIZE, 0x102);

    if (*path != '\0') {
        test = getFileIndex(path, parentIndex, dir);

        if (test == -1) {
            print(path);
            print(" tidak ada.\n");
            return;
        }

        parentIndex = test & 0xFF;
        if (*(dir + parentIndex + 1) != '\xFF') {
            print(path);
            print(" bukan direktori.\n");
            return;
        }
    }

    i = 0;
    while (i < 1024) {
        if (*(dir + i) == parentIndex && *(dir + i + 2) != 0) {
            if (!strncmp(dir + i + 2, "argv.tmp", 8)) {
                i += 16;
                continue;
            }
            print(dir + i + 2);
            if (*(dir + i + 1) == '\xFF') print("/");
            print("\n");
        }
        i += 16;
    }
}

char *getCwdName(char *cwdName, char cwdIdx) {
    char files[2 * SECTOR_SIZE];
    if (cwdIdx == '\xFF') {
        cwdName[0] = '/';
        cwdName[1] = 0;
    } else {

        getSector(files, 0x101);
        getSector(files + 512, 0x102);

        strncpy(cwdName, files + (cwdIdx * 16) + 2, 13);
    }

    return cwdName;
}
