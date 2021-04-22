#include "../lib/folderIO.h"
#include "../lib/utilities.h"
#include "../lib/string.h"

int main() {
    int argc; char argv[5][20];
    char parentIdx;
    char cwdIdx;
    char parents[6][14];
    char dir[512 * 2];
    int res;
    int success;
    int i;
    char *path;

    argc = getArguments(argv);
    cwdIdx = atoi(argv[0]) & 0xFF;

    res = strntoken(argv[1], parents, '/', 14);

    // mkdir a/b/c/d
    // argv[1] == "a/b/c/d"
    // hasil strntoken: parents == ["a", "b", "c", "d"]

    // menghilangkan folder paling akhir
    if (res > 1) {
        strncpy(path, parents[0], 13);

        for (i=1;i<res-1;i++) {
            strncat(path,"/",1);
            strncat(path,parents[i],13);
        }

        getSector(dir, 0x101);
        getSector(dir + 512, 0x102);

        parentIdx = getFileIndex(path, cwdIdx, dir);
    } else {
        parentIdx = cwdIdx;
    }

    createFolder(parents[res - 1], &success, parentIdx);
    if (success == -1) {
        print("Folder ");
        print(parents[res - 1]);
        print(" sudah ada");
        goto error;
    }

    exec_shell:
        sendArguments("", cwdIdx);
        exec("/bin/shell", 0x3002, 0, 0xFF);

    error:
    // print("Terjadi kesalahan saat menghapus file");
        goto exec_shell;

}