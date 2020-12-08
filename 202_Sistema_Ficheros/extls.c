#define _POSIX_C_SOURCE 200809L

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include "daverror.h"

int main(int argc, char ** argv) {
    char * path;
    int dfd;
    DIR * dir;
    off_t sum_size = 0;
    char currstr[FILENAME_MAX];
    char linkpath[FILENAME_MAX];
    struct dirent * curr;
    struct stat statbuf;

    if (argc > 1) {
        path = argv[1];
    } else {
        path = ".";
    }

    DAVO_CHECK(dfd = open(path, O_RDONLY | O_DIRECTORY));
    dir = fdopendir(dfd);
    curr = readdir(dir);
    while (curr != NULL) {
        // Hide dotfiles
        if (curr->d_name[0] != '.') {

            strncpy(currstr, path, FILENAME_MAX);
            strncat(currstr, "/", FILENAME_MAX-1);
            strncat(currstr, curr->d_name, FILENAME_MAX);

            DAVO_CHECK(lstat(currstr, &statbuf));

            switch (statbuf.st_mode & S_IFMT) {
                case S_IFDIR:
                    strncat(currstr, "/", FILENAME_MAX-1); 
                    break;
                case S_IFLNK: 
                    readlink(currstr, linkpath, FILENAME_MAX);
                    strncat(currstr, " -> ", FILENAME_MAX-4);
                    strncat(currstr, linkpath, FILENAME_MAX);
                    break;
                case S_IFREG:
                    sum_size += statbuf.st_size;
                    break;
                default: strncat(currstr, "?", FILENAME_MAX-1); break;
            }

            printf("%s\n", currstr);
        }

        curr = readdir(dir);
    }

    printf("Total size: %ldkb\n", sum_size / 1024);

    return EXIT_SUCCESS;
}
