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
    char command[10 * MAXIMUM_CMD_LEN];  // kalo pointer aja takut error
    char argvTmp[10 * MAXIMUM_CMD_LEN];
    char *argvStart;
    char argv[9][MAXIMUM_CMD_LEN];

    char hist[HIST_SIZE][10 * MAXIMUM_CMD_LEN];

    char cwdName[14], prompt[27];
    char cwdIdx = 0xFF;
    char cwdIdxStr[3];

    int argc, histc = 0, i;

    cwdName[0] = '/';
    cwdName[1] = 0;

    strncpy(prompt, "0xCABECABE", 10);
    strncat(prompt, "@", 1);
    strncat(prompt, cwdName, strlen(cwdName));
    strncat(prompt, "> ", 2);  // default prompt: "0xCABECABE@/> "

    while (true) {
        // set prompt
        clear(prompt + 11, 16);
        strncat(prompt, cwdName, strlen(cwdName));
        strncat(prompt, "> ", 2);
        printString(prompt);

        readString(command);
        if (*command == '\0') {
            continue;
        }

        // parse command
        argvStart = command;

        // cari tempat mulai nulis argumen, mau ada ataupun tidak
        while (*argvStart != ' ' && *argvStart != 0) {
            argvStart += 1 + (1 * (*argvStart == '\\'));
        }
        // isi ke argvTmp (buffer buat file argv.tmp)
        clear(argvTmp, strlen(argvTmp));
        // itoa(argvTmp, cwdIdx, 3);
        strncpy(argvTmp, &cwdIdx, 1); // is dis possible?
        strncat(argvTmp, " ", 1);
        strncat(argvTmp, argvStart + 1, strlen(argvStart + 1));
        // tulis argumen ke argv.tmp
        secSize = 1;
        // writeFile(argvTmp, "argv.tmp", &secSize, 0xFF);
        writeFile(argvStart + 1, "argv.tmp", &secSize, 0xFF);

        argc = strntoken(command, argv, ' ', MAXIMUM_CMD_LEN);

        // cut-off command
        *argvStart = 0;

        // eksekusi perintah
        if (strncmp("cd", argv[0], MAXIMUM_CMD_LEN) == 0) {
            if (argc != 2) {
                printString("Penggunaan: cd <path/ke/direktori>\n");
            } else {
                cd(&cwdIdx, argv[1], cwdName);
            }
        } else if (strncmp("ls", argv[0], MAXIMUM_CMD_LEN) == 0) {
            if (argc == 1) {
                listDir("", cwdIdx);
            } else if (argc == 2) {
                listDir(argv[1], cwdIdx);
            } else {
                printString("Penggunaan: ls [path/ke/direktori]");
            }
        } else if (strncmp("cat", argv[0], MAXIMUM_CMD_LEN) == 0) {
            if (argc != 2) {
                printString("Penggunaan: cat <path/file>\n");
            } else {
                cat(cwdIdx, argv[1]);
            }
        } else if (strncmp("ln", argv[0], MAXIMUM_CMD_LEN) == 0) {
            if (argc != 3) {
                printString(
                    "Penggunaan: ln <path/ke/sumber> <path/ke/tujuan>\n");
            } else {
                /*hardLink(cwdIdx, argv[1], argv[2]);*/
            }
        } else if (strncmp("cwd", argv[0], MAXIMUM_CMD_LEN) == 0) {
            printNumber(cwdIdx);
            printString(" - ");
            printString(cwdName);
            printString("\n");
        } else if (strncmp("history", argv[0], MAXIMUM_CMD_LEN) == 0) {
            for (i = 0; i < HIST_SIZE; i++) {
                if (strlen(hist[i]) != 0) {
                    printString(hist[i]);
                    printString("\n");
                }
            }
        } else if (strncmp("cp", argv[0], MAXIMUM_CMD_LEN) == 0) {
            if (argc != 3) {
                printString(
                    "Penggunaan: cp <path/ke/sumber> <path/ke/tujuan>\n");
            } else {
                cp(cwdIdx, argv[1], argv[2]);
            }
        } else if (strncmp("rm", argv[0], MAXIMUM_CMD_LEN) == 0) {
            if (argc != 2) {
                printString("Penggunaan: rm <path/file>\n");
            } else {
                /*rm(cwdIdx, argv[1]);*/
            }
        } else {
            printString("Perintah ");
            printString(argv[0]);
            printString(" tidak dikenali.\n");
        }

        // HISTORY
        histc = (histc >= HIST_SIZE) ? 0 : histc;
        for (i = 1; i < HIST_SIZE; i++) {
            strcpy(hist[i - 1], hist[i]);
        }
        strcpy(hist[HIST_SIZE - 1], command);
        histc++;

        // remove argv.tmp
    }
}

void cd(char *parentIndex, char *path, char *newCwdName) {
    char dir[2 * SECTOR_SIZE];
    int tmpPI = *parentIndex, test;
    bool found = false, isDir = true;

    if (strncmp(path, ".", MAXIMUM_CMD_LEN)) {
        if (strncmp(path, "/", MAXIMUM_CMD_LEN) != 0) {
            readSector(dir, 0x101);
            readSector(dir + SECTOR_SIZE, 0x102);

            test = getFileIndex(path, *parentIndex, dir);
            tmpPI = test & 0xFF;
            found = test > -1;
        } else {
            found = true;
            tmpPI = 0xFF;
        }

        if (found) {
            isDir = *(dir + (tmpPI * 0x10) + 1) == '\xFF';
            if (tmpPI == 0xFF) {  // cd ke root
                *parentIndex = 0xFF;
                strncpy(newCwdName, "/", 14);
            } else if (isDir) {
                *parentIndex = tmpPI;
                strncpy(newCwdName, dir + (tmpPI * 0x10) + 2, 14);
            } else {
                printString(path);
                printString(" bukan direktori.\n");
            }
        } else {
            printString("Direktori ");
            printString(path);
            printString(" tidak ditemukan.\n");
        }
    }
    return;
}

void listDir(char *path, char parentIndex) {
    char dir[2 * SECTOR_SIZE];
    int i, test;

    readSector(dir, 0x101);
    readSector(dir + SECTOR_SIZE, 0x102);

    if (*path != '\0') {
        test = getFileIndex(path, parentIndex, dir);

        if (test == -1) {
            printString(path);
            printString(" tidak ada.\n");
            return;
        }

        parentIndex = test & 0xFF;
        if (*(dir + parentIndex + 1) != '\xFF') {
            printString(path);
            printString(" bukan direktori.\n");
            return;
        }
    }

    i = 0;
    while (i < 1024) {
        if (*(dir + i) == parentIndex && *(dir + i + 2) != 0) {
            printString(dir + i + 2);
            if (*(dir + i + 1) == '\xFF') printString("/");
            printString("\n");
        }
        i += 16;
    }
}

// TODO: cek dia direktori apa file?
void cat(char cwdIdx, char *path) {
    char buf[16 * SECTOR_SIZE];
    int res = 0;

    readFile(buf, path, &res, cwdIdx);

    if (res > 0)
        printString(buf);
    else {
        printString("Terjadi kesalahan saat membaca berkas ");
        printString(path);
    }
    printString("\n");
}

void cp(char cwdIdx, char *resourcePath, char *destinationPath) {
    char buf[16 * SECTOR_SIZE];
    char dir[2 * SECTOR_SIZE];
    int res = 0;

    readSector(dir, 0x101);
    readSector(dir + SECTOR_SIZE, 0x102);

    readFile(buf, resourcePath, &res, cwdIdx);
    if (res <= 0) {  // read error
        printString("File ");
        printString(resourcePath);
        printString(" tidak ditemukan\n");
        return;
    }

    writeFile(buf, destinationPath, &res, cwdIdx);
    if (res <= 0) {  // write errror
        goto cp_error;
        return;
    }

    return;

cp_error:
    printString("Terjadi kesalahan saat menyalin file.\n");
    return;
}