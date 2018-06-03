#include <fcntl.h>    /* open */
#include <stdio.h> /* perror */
#include <stdlib.h> /* EXIT_*, size_t, ssize_t */
#include <unistd.h> /* read, write, close */
#include <stdbool.h> /* true, false */
#include <sys/stat.h> /* struct stat */
#include <sys/sendfile.h> /* sendfile */

#include "mycp.h"

/* man 2 sendfile:
 *
 * sendfile() will transfer at most 0x7ffff000 (2,147,479,552) bytes,
 * returning the number of bytes actually transferred.  (This is true on
 * both 32-bit and 64-bit systems.)
 */
#define MAX_SENDFILE_TRANSFER 2147479552


bool sendfile_cp(const char *from_filename, const char *to_filename) {
    struct stat status;
    int from_fd, to_fd;
    off_t transfer_size, filesize, offset = 0;
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

    if(-1==fstat(from_fd, &status)) {
        perror("fstat");
        goto close_all;
    }

    filesize = status.st_size;
    transfer_size = (filesize>MAX_SENDFILE_TRANSFER ? MAX_SENDFILE_TRANSFER : filesize);

    while(offset<filesize) {
        /* no check errors: try transfer data until finished. */
        sendfile(to_fd, from_fd, &offset, transfer_size);
    }

    if(-1==fsync(from_fd)) {
        perror("fsync()");
        goto close_all;
    }

    response = true;

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
