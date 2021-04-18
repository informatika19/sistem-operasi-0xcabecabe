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
    char *aaaaaaaaaaaaaaaaaaaa;
    char cwdIdxStr[3];

    int argc, histc = 0, i, execRes = 0, j = 0;

    cwdName[0] = '/';
    cwdName[1] = 0;

    strncpy(prompt, "0xCABECABE", 10);
    strncat(prompt, "@", 1);
    strncat(prompt, cwdName, strlen(cwdName));
    strncat(prompt, "> ", 2);  // default prompt: "0xCABECABE@/> "

    while (true) {
        // set prompt
        fillBuffer(prompt + 11, 16, 0);
        strncat(prompt, cwdName, strlen(cwdName));
        strncat(prompt, "> ", 2);
        print(prompt);

        read(command);
        if (*command == '\0') {
            continue;
        }

        // parse command
        argvStart = command;

        // cari tempat mulai nulis argumen, mau ada ataupun tidak
        while (*argvStart != ' ' && *argvStart != 0) {
            argvStart += 1 + (1 * (*argvStart == '\\'));
        }

        // remove argv.tmp
        removeFile("argv.tmp", 0, 0xFF);

        // bikin argv.tmp lagi, pertama isi ke argvTmp (buffer buat file argv.tmp)
        fillBuffer(argvTmp, strlen(argvTmp), 0);
        fillBuffer(cwdIdxStr, 3, 0);
        itoa(cwdIdxStr, (cwdIdx << 2) >> 2, 3);
        strncpy(argvTmp, cwdIdxStr, 3);
        strncat(argvTmp, " ", 1);
        strncat(argvTmp, argvStart + 1, strlen(argvStart + 1));
        updateFile(argvTmp, "argv.tmp", &secSize, 0xFF);

        argc = strntoken(command, argv, ' ', MAXIMUM_CMD_LEN);

        // cut-off command
        *argvStart = 0;

        // eksekusi perintah
        if (strncmp("cd", argv[0], MAXIMUM_CMD_LEN) == 0) {
            if (argc != 2) {
                print("Penggunaan: cd <path/ke/direktori>\n");
            } else {
                cd(&cwdIdx, argv[1], cwdName);
            }
        } else if (strncmp("ls", argv[0], MAXIMUM_CMD_LEN) == 0) {
            if (argc == 1) {
                listDir("", cwdIdx);
            } else if (argc == 2) {
                listDir(argv[1], cwdIdx);
            } else {
                print("Penggunaan: ls [path/ke/direktori]");
            }
        } else if (strncmp("cat", argv[0], MAXIMUM_CMD_LEN) == 0) {
            if (argc != 2) {
                print("Penggunaan: cat <path/file>\n");
            } else {
                cat(cwdIdx, argv[1]);
            }
        } else if (strncmp("ln", argv[0], MAXIMUM_CMD_LEN) == 0) {
            if (argc != 3) {
                print(
                    "Penggunaan: ln <path/ke/sumber> <path/ke/tujuan>\n");
            } else {
                /*hardLink(cwdIdx, argv[1], argv[2]);*/
            }
        } else if (strncmp("cwd", argv[0], MAXIMUM_CMD_LEN) == 0) {
            printNumber(cwdIdx);
            print(" - ");
            print(cwdName);
            print("\n");
        } else if (strncmp("history", argv[0], MAXIMUM_CMD_LEN) == 0) {
            for (i = 0; i < HIST_SIZE; i++) {
                if (strlen(hist[i]) != 0) {
                    print(hist[i]);
                    print("\n");
                }
            }
        } else if (strncmp("cp", command, MAXIMUM_CMD_LEN) == 0) {
            exec("/bin/cp", 0x1C00, 0, 0xFF);
        } else if (strncmp("rm", argv[0], MAXIMUM_CMD_LEN) == 0) {
            if (argc != 2) {
                print("Penggunaan: rm <path/file>\n");
            } else {
                /*rm(cwdIdx, argv[1]);*/
            }
        } else {
            exec("a", 0x6700, &execRes, 0xFF);
            if (!execRes) {
                print("Perintah ");
                print(argv[0]);
                print(" tidak dikenali.\n");
            }
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

    if (strncmp(path, ".", MAXIMUM_CMD_LEN)) {
        if (strncmp(path, "/", MAXIMUM_CMD_LEN) != 0) {
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
            if (tmpPI == 0xFF) {  // cd ke root
                *parentIndex = 0xFF;
                strncpy(newCwdName, "/", 14);
            } else if (isDir) {
                *parentIndex = tmpPI;
                strncpy(newCwdName, dir + (tmpPI * 0x10) + 2, 14);
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

// TODO: cek dia direktori apa file?
void cat(char cwdIdx, char *path) {
    char buf[16 * SECTOR_SIZE];
    int res = 0;

    getFile(buf, path, &res, cwdIdx);

    if (res > 0)
        print(buf);
    else {
        print("Terjadi kesalahan saat membaca berkas ");
        print(path);
    }
    print("\n");
}

void cp(char cwdIdx, char *resourcePath, char *destinationPath) {
    char buf[16 * SECTOR_SIZE];
    char dir[2 * SECTOR_SIZE];
    int res = 0;

    getSector(dir, 0x101);
    getSector(dir + SECTOR_SIZE, 0x102);

    getFile(buf, resourcePath, &res, cwdIdx);
    if (res <= 0) {  // read error
        print("File ");
        print(resourcePath);
        print(" tidak ditemukan\n");
        return;
    }

    updateFile(buf, destinationPath, &res, cwdIdx);
    if (res <= 0) {  // write errror
        goto cp_error;
        return;
    }

    return;

cp_error:
    print("Terjadi kesalahan saat menyalin file.\n");
    return;
}
