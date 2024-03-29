/**
 * shell.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 15 Maret 2021
 *
 * Header program shell untuk OS 0xCABECABE
 */

#ifndef _SHELL_CABECABE_H_
#define _SHELL_CABECABE_H_

#define MAXIMUM_CMD_LEN 10
#define HIST_SIZE 5

void cd(char *parentIndex, char *path, char *newCwdName);

void listDir(char *path, char parentIndex);

void cat(char parentIndex, char *path);

void cp(char cwdIdx, char *resourcePath, char *destinationPath);

char *getCwdName(char *cwdName, char cwdIdx);

#endif
