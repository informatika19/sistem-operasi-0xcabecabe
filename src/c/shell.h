#ifndef _SHELL_CABECABE_H_
#define _SHELL_CABECABE_H_

#define SECTOR_SIZE 512

#define MAXIMUM_CMD_LEN 20

int runShell();

int commandParser(char *cmd, char *argument);

void cd(char *parentIndex, char *path, char *newCwdName, char *dir);

void listDir(char parentIndex, char *dir);

#endif