#include <fcntl.h>    /* open */
#include <stdio.h> /* perror */
#include <stdlib.h> /* exit, EXIT_*, size_t, ssize_t */
#include <sys/stat.h> /* struct stat */

#include "util.h"


int open_from(const char *filename) {
    int fd;

    fd = open(filename, O_RDONLY);

    if(-1==fd) {
        perror("Cannot open 'from' file");
        exit(EXIT_FAILURE);
    }

    return(fd);
}

int open_to(const char *filename) {
    int fd;

    fd = open(filename, O_RDWR|O_CREAT|O_TRUNC, S_IWUSR|S_IRUSR);

    if(-1==fd) {
        perror("Cannot create 'to' file");
        exit(EXIT_FAILURE);
    }

    return(fd);
}

size_t best_block_size(void) {
    struct stat stats;

    if (!stat(".", &stats)) {
        return stats.st_blksize;
    }

    return 512;
}
