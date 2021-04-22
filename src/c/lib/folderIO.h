/**
 * folderIO.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 2 April 2021
 *
 * Library untuk operasi pada directory
 */

#ifndef _folderIO_CABECABE_H_
#define _folderIO_CABECABE_H_
#include "fileIO.h"

/**
 * @param path path atau nama file yang ingin dibuat
 * @param res jumlah dari dir yang akan dibuat
 * @param success hasil operasi
 *                -1 : operasi gagal
 *                 0 : operasi berhasl
 * @param parentIndex "current working directory" index, lokasi relatif terhadap path
 * @param parents string hasil parsing path
 */
void createFolder(char *fileName, int *success, char parentIndex);

/**
 * @param path path atau nama file yang ingin dihapus
 * @param success hasil operasi
 *                -1 : operasi gagal
 *                 0 : operasi berhasil
 * @param parentIndex "current working directory" index, lokasi relatif terhadap path
 */
void removeFolder(char *path, int *success, char parentIndex);

#endif
