/**
 * shell.c
 * Alvin W., Josep M., Rehagana K.C.S.
 * 15 Maret 2021
 *
 * Implementasi program shell untuk OS 0xCABECABE
 */

#include "shell.h"

#include "filesystem.h"  // parsePath, SECTOR_SIZE
#include "io.h"          // printNumber
#include "kernel.h"
#include "lib/boolean.h"
#include "lib/string.h"

int runShell() {
    char command[10 * MAXIMUM_CMD_LEN];  // kalo pointer aja takut error
    char argv[10][MAXIMUM_CMD_LEN];

    char cwdIdx = 0xFF, username[11],
         cwdName[14],  // root
        promptHead[3], prompt[27], atSymb[2];
    // hist[HIST_SIZE][10*MAXIMUM_CMD_LEN];

    int argc, histCounter = 0, histTail = 0, i;

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
        handleInterrupt21(0, prompt, 0, 0);

        // baca perintah dan simpan di history
        handleInterrupt21(1, command, 0, 0);
        /* History
        histTail = histTail == 3 ? histTail : histTail+1;
        histCounter++;
        if (histCounter >= 3)
        {
            for (i = HIST_SIZE-1; i >= 0; --i)
                strncpy(hist[i+1], hist[i], 10*MAXIMUM_CMD_LEN);
            strncpy(hist[HIST_SIZE-1], command, 10*MAXIMUM_CMD_LEN);
        }
        else
            strncpy(hist[histCounter-1], command, 10*MAXIMUM_CMD_LEN);
        */

        // parse dan hasil parse
        argc = commandParser(command, argv);
        if (argc < 0) {
            // TODO: bad UX because doesn't tell the error
            handleInterrupt21(0, "Terjadi kesalahan saat membaca perintah\n", 0,
                              0);
            continue;
        }

        // eksekusi perintah
        if (strncmp("cd", argv[0], MAXIMUM_CMD_LEN) == 0) {
            if (argc != 2) {
                handleInterrupt21(
                    0, "Penggunaan: cd <path/direktori>\n", 0, 0);
            } else {
                cd(&cwdIdx, argv[1], cwdName);
            }
        } else if (strncmp("ls", argv[0], MAXIMUM_CMD_LEN) == 0) {
            listDir(cwdIdx);
        } else if (strncmp("cat", argv[0], MAXIMUM_CMD_LEN) == 0) {
            if (argc != 2) {
                handleInterrupt21(0,
                                  "Penggunaan: cat <path/file>\n",
                                  0, 0);
            } else {
                cat(cwdIdx, argv[1]);
            }
        } else if (strncmp("ln", argv[0], MAXIMUM_CMD_LEN) == 0) {
            // pseudo-ln KeK
            if (argc != 3 && argc != 4) {
                printNumber(argc);
                printString("\n");
                handleInterrupt21(0,
                                  "Penggunaan: ln [-s] <path/sumber> <path/tujuan>\n",
                                  0,0);
            } else if (argc == 4 && strncmp(argv[1], "-s", MAXIMUM_CMD_LEN) == 0) {
                softLink(cwdIdx, argv[2], argv[3]);
            } else {
                hardLink(cwdIdx, argv[1], argv[2]);
            }
        } else if (strncmp("cwd", argv[0], MAXIMUM_CMD_LEN) == 0) {
            printNumber(cwdIdx);
            printString(" - ");
            printString(cwdName);
            printString("\n");
        } else {
            handleInterrupt21(0, "Perintah ", 0, 0);
            handleInterrupt21(0, argv[0], 0, 0);
            handleInterrupt21(0, " tidak dikenali.\n", 0, 0);
        }
    }
}

// TODO: tanganin spasi, ", dan '
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
                stop = cmd == 0; // \ di akhir string
                i += 1 * !stop;
                break;
            // case '\'':
            // case '"':
            //     // treat ' and " as the same for now, meaning ' can be
            //     // ended with " and vice versa
            //     // TODO: masih ngebug aneh pisan hadeuh & harus handle: \' \"
            //     cmd++;
            //     while (!stop && *cmd != '\0' && (*cmd != '\'' || *cmd != '"')) {
            //         printNumber(*cmd);
            //         printString("\n");
            //         *(argument + j + i) = *cmd;
            //         cmd++;
            //         i++;
            //         // kasus ngewrite sampe ujung tapi ga diakhiri ' atau "
            //         stop = *cmd == '\0' || i >= MAXIMUM_CMD_LEN;
            //     }
            //     stop = i >= MAXIMUM_CMD_LEN;
            //     j += MAXIMUM_CMD_LEN * (i != 0);
            //     i = 0;
            //     break;
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
    int tmpPI = *parentIndex;
    bool found = false, isDir = true;

    handleInterrupt21(0x0002, dir, 0x101, 0);  // readSector
    handleInterrupt21(0x0002, dir + 512, 0x102, 0);

    tmpPI = getFileIndex(path, *parentIndex, dir);
    found = tmpPI > -1;

    if (found) {
        isDir = *(dir + (tmpPI * 0x10) + 1) == '\xFF';
        if (tmpPI == 0xFF) {
            *parentIndex = 0xFF;
            strncpy(newCwdName, "/",  14);
        } else if (isDir) {
            *parentIndex = tmpPI;
            strncpy(newCwdName, dir + (tmpPI * 0x10) + 2, 14);
        } else {
            handleInterrupt21(0, path, 0, 9);
            handleInterrupt21(0, " bukan direktori.\n", 0, 0);
        }
    } else {
        handleInterrupt21(0, "Direktori ", 0, 9);
        handleInterrupt21(0, path, 0, 0);
        handleInterrupt21(0, " tidak ditemukan.\n", 0, 0);
    }
    return;
}

void listDir(char parentIndex) {
    int i = 0;
    char dir[2 * SECTOR_SIZE];

    handleInterrupt21(0x0002, dir, 0x101, 0);  // readSector
    handleInterrupt21(0x0002, dir + 512, 0x102, 0);

    while (i < 1024) {
        if (*(dir + i) == parentIndex && *(dir+i+2) != 0) {
            handleInterrupt21(0, dir + i + 2, 0, 0);
            handleInterrupt21(0, "\n", 0, 0);
        }
        i += 16;
    }
}

// TODO: cek dia direktori apa file?
void cat(char cwdIdx, char *path) {
    char buf[16 * SECTOR_SIZE];
    int res = 0;

    handleInterrupt21((cwdIdx << 8) + 0x04, buf, path, &res);

    if (res > 0)
        handleInterrupt21(0, buf, 0, 0);
    else {
        handleInterrupt21(0, "Terjadi kesalahan saat membaca berkas ", 0, 0);
        handleInterrupt21(0, path, 0, 0);
    }
    handleInterrupt21(0, "\n", 0, 0);
}

void hardLink(char cwdIdx, char *resourcePath, char *destinationPath) {
    char buf[16* SECTOR_SIZE];
    int res = 0;

    handleInterrupt21((cwdIdx << 8) + 0x04, buf, resourcePath, &res);

    if (res > 0) {
        handleInterrupt21((cwdIdx << 8) + 0x05, buf, destinationPath, &res);
    } else {
        handleInterrupt21(0, "Terjadi kesalahan saat membaca berkas ", 0, 0);
        handleInterrupt21(0, resourcePath, 0, 0);
    }
}

void softLink(char cwdIdx, char *resourcePath, char *destinationPath) {
    char dir[2*SECTOR_SIZE];

    int destinationIndex = getFileIndex(destinationPath, cwdIdx, dir);
    int resourceIndex = getFileIndex(resourcePath, cwdIdx, dir);
    int i = 0;
    int j = resourceIndex*0x10;
    char fname[14];
    char parents[64][14];
    int testo;

    handleInterrupt21(0x0002, dir, 0x101, 0);  // readSector
    handleInterrupt21(0x0002, dir + 512, 0x102, 0);

    testo = parsePath(destinationPath,parents, fname);
    if (destinationIndex == -1){
        while (*(dir+i) != 0){
            i+=0x10;
        }
        *(dir+i) = cwdIdx;
        *(dir+i+1) = *(dir+j+1);
        printString(fname);
        printString("\n");
        strncpy(dir+i+2, fname, 14);
    }

    handleInterrupt21(0x0003, dir, 0x101, 0);  // writeSector
    handleInterrupt21(0x0003, dir + 512, 0x102, 0);
}