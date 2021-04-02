/**
 * shell.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 15 Maret 2021
 *
 * Implementasi program shell untuk OS 0xCABECABE
 */

#include "shell.h"

#include "../lib.h"

int runShell() {
    char command[10 * MAXIMUM_CMD_LEN];  // kalo pointer aja takut error
    char argv[10][MAXIMUM_CMD_LEN];

    char hist[HIST_SIZE][10 * MAXIMUM_CMD_LEN];

    char username[11], cwdName[14], promptHead[3], prompt[27], atSymb[2];
    char cwdIdx = 0xFF;

    int argc, histc = 0, i;

    strncpy(username, "0xCABECABE", 11);
    atSymb[0] = '@';
    atSymb[1] = 0;
    cwdName[0] = '/';
    cwdName[1] = 0;
    promptHead[0] = '>';
    promptHead[1] = ' ';
    promptHead[2] = 0;  // default prompt: "0xCABECABE@/> "

    while (true) {
        // set prompt
        clear(prompt, 27);
        strncat(prompt, username, strlen(username));
        strncat(prompt, atSymb, 1);
        strncat(prompt, cwdName, strlen(cwdName));
        strncat(prompt, promptHead, 2);
        printString(prompt);

        readString(command);

        // parse dan hasil parse
        argc = commandParser(command, argv);
        if (argc < 0) {
            // TODO: bad UX because doesn't tell the error
            printString("Terjadi kesalahan saat membaca perintah\n");
            continue;
        }

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
                printString("Penggunaan: ln <path/ke/sumber> <path/ke/tujuan>\n");
            } else {
                hardLink(cwdIdx, argv[1], argv[2]);
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
                printString("Penggunaan: cp <path/ke/sumber> <path/ke/tujuan>\n");
            } else {
                cp(cwdIdx, argv[1], argv[2]);
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
    }
}

int commandParser(char *cmd, char *argument) {
    int i, j;
    bool stop = false;

    i = 0, j = 0;
    for (; *cmd == ' '; cmd++)
        ;
    while (*cmd != '\0' && !stop) {
        stop = i >= MAXIMUM_CMD_LEN;
        switch (*cmd) {
            case ' ':
                *(argument + j + i) = 0;
                j += MAXIMUM_CMD_LEN * (i != 0);
                i = 0;
                break;
            case '\\':
                cmd++;
                *(argument + j + i) = *(cmd);
                stop = cmd == 0;  // \ di akhir string
                i += 1 * !stop;
                break;
            default:
                *(argument + j + i) = *cmd;
                i++;
        }

        cmd++;
    }

    *(argument + j + i) = 0;

    return stop ? -1 : (div(j, MAXIMUM_CMD_LEN) + 1);
}

void cd(char *parentIndex, char *path, char *newCwdName) {
    char dir[2 * SECTOR_SIZE];
    int tmpPI = *parentIndex, test;
    bool found = false, isDir = true;

    // TODO: ga boleh langsung pake interrupt
    readSector(dir, 0x101);
    readSector(dir + SECTOR_SIZE, 0x102);

    if (strncmp(path, ".", MAXIMUM_CMD_LEN)) {
        if (strncmp(path, "/", MAXIMUM_CMD_LEN)) {
            test = getFileIndex(path, *parentIndex);
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
    char children[64][15];
    int res = 0;

    res = getChildrenFiles(path, parentIndex, children);
    if (res == -1) {
        printString(path);
        printString(" bukan direktori.\n");
        return;
    } else if (res == -2) {
        printString(path);
        printString(" tidak ada.\n");
        return;
    } else if (res < 0) {
        printString("Terjadi kesalahan.\n");
    }

    while(res--) {
        printString(children[res]);
        printString("\n");
    }
}

// TODO: cek dia direktori apa file?
void cat(char cwdIdx, char *path) {
    char buf[16 * SECTOR_SIZE];
    int res = 0;

    readFile(buf, path, &res, cwdIdx);

    if (res > 0) {
        printString(buf);
    } else {
        printString("Terjadi kesalahan saat membaca berkas ");
        printString(path);
    }
    printString("\n");
}

// TODO: cek yang mau di-link file apa dir
void cp(char cwdIdx, char *resourcePath, char *destinationPath) {
    char buf[16 * SECTOR_SIZE];
    int res = 0;

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

// TODO: cek yang mau di-link file apa dir
void hardLink(char cwdIdx, char *resourcePath, char *destinationPath) {
    char dir[2 * SECTOR_SIZE];

    int testDI, testRI, i = 0, jmlParents = 0;
    char destinationIndex, resourceIndex;
    char fname[14];
    char parents[64][14];
    int tmp = 2 * SECTOR_SIZE;

    testDI = getFileIndex(destinationPath, cwdIdx);
    testRI = getFileIndex(resourcePath, cwdIdx);
    destinationIndex = testDI & 0xFF;
    resourceIndex = testRI & 0xFF;

    if (testDI == -1 && testRI != -1) {
        jmlParents = tokenize(destinationPath, parents, '/');
        strncpy(fname, parents[jmlParents - 1], 14);
        if (jmlParents != 0) {
            clear(destinationPath, strlen(destinationPath));
            strncpy(destinationPath, parents[0], strlen(parents[0]));
            strncat(destinationPath, "/", 14);
            for (i = 1; i < jmlParents; ++i) {
                strncat(destinationPath, parents[i], strlen(parents[i]));
                strncat(destinationPath, "/", 2);
            }
            cwdIdx = getFileIndex(destinationPath, cwdIdx) & 0xFF;
        }

        i = 0;
        while (*(dir + i + 2) != 0 && i < tmp) {
            i += 0x10;
        }
        if (*(dir + i + 2) != 0) {  // sektor files penuh
            printString("sektor penuh\n");
            goto hardLink_error;
            return;
        }

        *(dir + i) = cwdIdx;
        *(dir + i + 1) = *(dir + resourceIndex * 0x10 + 1);
        strncpy(dir + i + 2, fname, 14);

        // TODO: ga boleh pake interrupt langsung
        interrupt(0x21, 0x0003, dir, 0x101, 0);  // writeSector
        interrupt(0x21, 0x0003, dir + 512, 0x102, 0);

        return;
    } else {
        goto hardLink_error;
        return;
    }

hardLink_error:
    printString("Terjadi kesalahan saat membuat symbolic link\n");
    return;
}
