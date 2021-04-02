/**
 * folderIO.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 2 April 2021
 *
 * Library untuk operasi pada directory
 */

#ifndef _folderIO_CABECABE_H_
#define _folderIO_CABECABE_H_

/**
 * Fungsi untuk mendapatkan child files and directories dari suatu
 * folder
 * @param path string path relatif ke folder
 * @param parentIndex int parent index dari path
 * @param res array of strings berisi nama children files dan directories
 * @return mengembalikan banyak children
 */
int getChildrenFiles(char *path, int parentIndex, char *res);

#endif
