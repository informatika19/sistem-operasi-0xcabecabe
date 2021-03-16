#include "filesystem.h"
#include "shell.h"
#include "kernel.h"
#include "io.h"
#include "lib.h"

int runShell()
{
    char command[10*MAXIMUM_CMD_LEN]; // kalo pointer aja takut error
    char arguments[10][MAXIMUM_CMD_LEN];

    char cwdIdx = 0xFF,
         username[11],
         cwdName[14], //root
         promptHead[3],
         prompt[27],
         atSymb[2],
         hist[HIST_SIZE][10*MAXIMUM_CMD_LEN];

    int res,
        histCounter = 0,
        histTail = 0,
        i;

    strncpy(username, "0xCABECABE", 11);
    atSymb[0] = '@';
    atSymb[1] = 0;
    cwdName[0] = '/';
    cwdName[1] = 0;
    promptHead[0] = '>';
    promptHead[1] = ' ';
    promptHead[2] = 0; // default prompt: "0xCABECABE@/> "

    while (true)
    {
        // set prompt
        clear(prompt, 27);
        strncat(prompt, username, strlen(username));
        strncat(prompt, atSymb, 1);
        strncat(prompt, cwdName, strlen(cwdName));
        strncat(prompt, promptHead, 2);
        printString(prompt);

        // baca perintah dan simpan di history
        readString(command);
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

        // parse dan hasil parse
        res = commandParser(command, arguments);
        if (res < 0)
        {
            printString("Terjadi kesalahan saat membaca perintah\n");
            printString("Panjang maksimal sebuah perintah/argumen perintah adalah ");
            printNumber(MAXIMUM_CMD_LEN);
            printString(" karakter.\n");
            continue;
        }

        // eksekusi perintah
        if (strncmp("cd", arguments[0], MAXIMUM_CMD_LEN) == 0)
        {
            if (strlen(arguments[1]) == 0)
            {
                printString("Perintah cd membutuhkan sebuah direktori.\n");
            }
            else
            {
                cd(&cwdIdx, arguments[1], cwdName);
            }
        }
        else if (strncmp("ls", arguments[0], MAXIMUM_CMD_LEN) == 0)
        {
            listDir(cwdIdx);
        }
        else if (strncmp("cat", arguments[0], MAXIMUM_CMD_LEN) == 0)
        {
            cat(cwdIdx, arguments[1]);
        }
        else if (strncmp("ln", arguments[0], MAXIMUM_CMD_LEN) == 0)
        {
        }
        else if (strncmp("history", arguments[0], MAXIMUM_CMD_LEN) == 0)
        {
            for (i = 0; i < histTail; ++i)
            {
                printString(hist[i]);
                printString("\n");
            }
        }
        else
        {
            printString("Perintah ");
            printString(arguments[0]);
            printString(" tidak dikenali.\n");
        }
    }
}

int commandParser(char *cmd, char *argument)
{
    int i, j;
    bool stop = false;

    i = 0, j = 0;
    for (; *cmd == ' '; cmd++);
    while (*cmd != '\0' && !stop)
    {
        stop = i >= MAXIMUM_CMD_LEN;
        switch (*cmd)
        {
            case ' ':
                *(argument+j+i) = 0;
                j += MAXIMUM_CMD_LEN * (i != 0);
                i = 0;
                break;
            default:
                *(argument+j+i) = *cmd;
                i++;
        }

        cmd++;
    }

    *(argument+j+i) = 0;

    return stop ? -1 : div(j, MAXIMUM_CMD_LEN);

}

void cd(char *parentIndex, char *path, char *newCwdName) {
    char dir[2*SECTOR_SIZE];
    char parents[64][14], fname[14];
    int newIndex = 0xFF;
    int res, i, tmp = 2*SECTOR_SIZE, tmpPI = *parentIndex;
    bool found = false, isDir = true;

    handleInterrupt21(0x0002, dir, 0x101, 0); // readSector
    handleInterrupt21(0x0002, dir+512, 0x102, 0);

    if (*path == '/')
        tmpPI = 0xFF;

    res = parsePath(path, parents, fname);
    found = res == 0;

    i = 0;
    while (i < res)
    {
        newIndex = 0;
        while (newIndex < tmp && !found)
        {
            found = *(dir+newIndex) == tmpPI &&
                    strncmp(dir+newIndex+2, parents[i], 14) == 0;
            newIndex += 0x10;
        }

        if (found)
            tmpPI = newIndex - 0x10;
        else
            break;
        ++i;
    }

    if (found)
    {
        found = false;
        newIndex = 0;
        while (newIndex < tmp && !found)
        {
            found = *(dir+newIndex*0x10) == tmpPI &&
                    strncmp(dir+newIndex+2, fname, 14) == 0;
            isDir = *(dir+newIndex+1) > 0x1F;
            newIndex += 0x10;
        }
        newIndex -= 0x10;

        if (found && isDir)
        {
            tmpPI = newIndex/0x10;
            *parentIndex = tmpPI;
            strncpy(newCwdName, dir+newIndex+2, 14);
        }
        else if (found && !isDir)
        {
            printString(path);
            printString(" bukan direktori.\n");
        }
        else if (!found)
            goto CD_DIR_NOT_FOUND;
    }
    else
        goto CD_DIR_NOT_FOUND;

    return;

CD_DIR_NOT_FOUND:
    printString("Direktori ");
    printString(path);
    printString(" tidak ditemukan.\n");
    return;
}

void listDir(char parentIndex) {
    int i = 0;
    char dir[2*SECTOR_SIZE];

    handleInterrupt21(0x0002, dir, 0x101, 0); // readSector
    handleInterrupt21(0x0002, dir+512, 0x102, 0);
    while(i < 1024) {
        if (*(dir+i) == parentIndex){
            printString(dir+i+2);
            printString("\n");
        }
        i += 16;
    }
}

void cat(char cwdIdx, char *path)
{
    char buf[16*SECTOR_SIZE];
    int res = 0;

    readFile(buf, path, &res, cwdIdx);

    if (res > 0)
        printString(buf);
    else
    {
        printString("Terjadi kesalahan saat membaca berkas ");
        printString(path);
        printString("\n");
    }
}
