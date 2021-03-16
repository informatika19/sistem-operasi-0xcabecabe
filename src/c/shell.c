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
    char arguments[10][MAXIMUM_CMD_LEN];

    char cwdIdx = 0xFF, username[11],
         cwdName[14],  // root
        promptHead[3], prompt[27], atSymb[2];
    // hist[HIST_SIZE][10*MAXIMUM_CMD_LEN];

    int res, histCounter = 0, histTail = 0, i;

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
        res = commandParser(command, arguments);
        if (res < 0) {
            handleInterrupt21(0, "Terjadi kesalahan saat membaca perintah\n", 0,
                              0);
            handleInterrupt21(
                0, "Panjang maksimal sebuah perintah/argumen perintah adalah ",
                0, 0);
            printNumber(MAXIMUM_CMD_LEN);
            handleInterrupt21(0, " karakter.\n", 0, 0);
            continue;
        }

        // eksekusi perintah
        if (strncmp("cd", arguments[0], MAXIMUM_CMD_LEN) == 0) {
            if (strlen(arguments[1]) == 0) {
                handleInterrupt21(
                    0, "Perintah cd membutuhkan sebuah direktori.\n", 0, 0);
            } else {
                cd(&cwdIdx, arguments[1], cwdName);
            }
        } else if (strncmp("ls", arguments[0], MAXIMUM_CMD_LEN) == 0) {
            listDir(cwdIdx);
        } else if (strncmp("cat", arguments[0], MAXIMUM_CMD_LEN) == 0) {
            if (strlen(arguments[1]) == 0) {
                handleInterrupt21(0,
                                  "Perintah cat membutuhkan sebuah file "
                                  "sebagai argumennya.\n",
                                  0, 0);
            } else {
                cat(cwdIdx, arguments[1]);
            }
        } else if (strncmp("ln", arguments[0], MAXIMUM_CMD_LEN) == 0) {
        }
        /*
        else if (strncmp("history", arguments[0], MAXIMUM_CMD_LEN) == 0)
        {
            for (i = 0; i < histTail; ++i)
            {
                printString(hist[i]);
                printString("\n");
            }
        }
        */
        else {
            handleInterrupt21(0, "Perintah ", 0, 0);
            handleInterrupt21(0, arguments[0], 0, 0);
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
            default:
                *(argument + j + i) = *cmd;
                i++;
        }

        cmd++;
    }

    *(argument + j + i) = 0;

    return stop ? -1 : div(j, MAXIMUM_CMD_LEN);
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
        if (isDir) {
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
        if (*(dir + i) == parentIndex) {
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
