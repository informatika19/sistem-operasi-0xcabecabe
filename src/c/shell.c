#include "shell.h"
#include "kernel.h"
#include "lib.h"

int runShell()
{
    char command[500]; // kalo pointer aja takut error
    char arguments[25][MAXIMUM_CMD_LEN];

    char cwdIdx = 0xFF;
    char username[11];
    char cwdName[14]; //root
    char promptHead[3];
    char prompt[27];
    char dir[2*SECTOR_SIZE];
    char atSymb[2];

    strncpy(username, "0xCABECABE", 11);
    atSymb[0] = '@';
    atSymb[1] = 0;
    cwdName[0] = '/';
    cwdName[1] = '\0';
    promptHead[0] = '>';
    promptHead[1] = ' ';
    promptHead[2] = 0; // default prompt: "0xCABECABE@/> "

    handleInterrupt21(0x0002, dir, 0x101, 0);
    handleInterrupt21(0x0002, dir+512, 0x102, 0);

    while (true)
    {
        strncat(prompt, username, strlen(username));
        strncat(prompt, atSymb, 1);
        strncat(prompt, cwdName, strlen(cwdName));
        strncat(prompt, promptHead, 2);

        printString(prompt);
        readString(command);
        commandParser(command, arguments);

        if (strncmp("cd", arguments[0], MAXIMUM_CMD_LEN) == 0)
        {
            if (strlen(arguments[1]) == 0)
            {
                printString("Cannot cd to nothing.\n");
            }
            else
            {
                cd(&cwdIdx, arguments[1], cwdName, dir);
            }
        }
        else if (strncmp("ls", arguments[0], MAXIMUM_CMD_LEN) == 0)
        {
            listDir(cwdIdx, dir);
        }
        else
        {
            printString("Command ");
            printString(arguments[0]);
            printString(" not recognized.\n");
        }

        clear(prompt, 27);
    }
}

int commandParser(char *cmd, char *argument)
{
    int i, j;

    i = 0, j = 0;
    for (; *cmd == ' '; cmd++);
    while (*cmd != '\0')
    {
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

    return div(j, MAXIMUM_CMD_LEN);

}

void cd(char *parentIndex, char *path, char *newCwdName, char *dir) {
    char parents[64][14], fname[14];
    int newIndex = 0xFF;
    int res, i, tmp = 2*SECTOR_SIZE, tmpPI = *parentIndex;
    bool found = false, isDir = true;

    if (*path == '/')
        tmpPI = 0xFF;

    res = parsePath(path, parents, fname, parentIndex, dir);
    found = res == 0;

    i = 0;
    // include/lib/asd
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
            strncmp(newCwdName, dir+newIndex, 14);
        }
        else if (found && !isDir)
        {
            printString(path);
            printString(" is not a directory.\n");
        }
        else if (!found)
            goto DIR_NOT_FOUND;
    }
    else
        goto DIR_NOT_FOUND;

    return;

DIR_NOT_FOUND:
    printString("Directory ");
    printString(path);
    printString(" not found.\n");
    return;
}

void listDir(char parentIndex, char *dir) {
    int i;

    i = 0;
    while(i < 1024) {
        if (*(dir+i) == parentIndex){
            printString(dir+i+2);
            printString("\n");
        }
        i += 16;
    }
}