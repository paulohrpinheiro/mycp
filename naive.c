#include <fcntl.h>    /* open */
#include <stdio.h> /* perror */
#include <stdlib.h> /* EXIT_*, size_t, ssize_t */
#include <unistd.h> /* read, write, close */
#include <stdbool.h> /* true, false */
#include <sys/stat.h> /* struct stat */

#include "mycp.h"


bool naive_cp(const char *from_filename, const char *to_filename) {
    struct stat stats;
    int from_fd, to_fd;
    size_t block_size;
    ssize_t readed, writed;
    char *buffer;
    bool response = false;

    from_fd = open(from_filename, O_RDONLY);
    if(-1==from_fd) {
        perror("from file");
        goto finish;
    }

    to_fd = open(to_filename, O_RDWR|O_CREAT|O_TRUNC, S_IWUSR|S_IRUSR);
    if(-1==to_fd) {
        perror("to file");
        goto close_from;
    }

    if(!stat(".", &stats)) {
        block_size = stats.st_blksize;
    } else {
        block_size = 512;
    }

    buffer = (char *)malloc(block_size);
    if(NULL==buffer) {
        perror("malloc");
        goto close_all;
    }

    while(0 != (readed = read(from_fd, buffer, block_size))) {
        if(-1==readed) {
            perror("read from_fd");
            goto clean_all;
        }

        writed = write(to_fd, buffer, readed);

        if(-1==writed) {
            perror("write to_fd");
            goto clean_all;
        }

        if(writed!=readed) {
            perror("writed <> readed");
            goto clean_all;
        }
    }

    if(-1==fsync(from_fd)) {
        perror("fsync()");
        goto clean_all;
    }

    response = true;

clean_all:
    free(buffer);

close_all:
    if(-1==close(to_fd)) {
        perror("to_fd");
    }

close_from:
    if(-1==close(from_fd)) {
        perror("from_fd");
    }

finish:
    return(response);
}

