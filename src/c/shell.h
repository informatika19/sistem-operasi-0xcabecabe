/**
 * shell.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 15 Maret 2021
 *
 * Header program shell untuk OS 0xCABECABE
 */

#ifndef _SHELL_CABECABE_H_
#define _SHELL_CABECABE_H_

#define MAXIMUM_CMD_LEN 20
#define HIST_SIZE 5

int runShell();

int commandParser(char *cmd, char *argument);

void cd(int *parentIndex, char *path, char *newCwdName);

void listDir(int parentIndex);

void cat(int parentIndex, char *path);

void hardLink(int cwdIdx, char *resourcePath, char *destinationPath);

void softLink(int cwdIdx, char *resourcePath, char *destinationPath);

#endif
