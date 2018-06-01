#include <stdio.h> /* perror */
#include <stdlib.h> /* EXIT_*, size_t, ssize_t */
#include <unistd.h> /* read, write, close */
#include <stdbool.h> /* true, false */
#include <sys/stat.h> /* struct stat */
#include <sys/sendfile.h> /* sendfile */

#include "mycp.h"
#include "util.h"

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

    from_fd = open_from(from_filename);
    to_fd = open_to(to_filename);

    fstat(from_fd, &status);
    filesize = status.st_size;
    transfer_size = (filesize>MAX_SENDFILE_TRANSFER ? MAX_SENDFILE_TRANSFER : filesize);

    while(offset<filesize) {
        sendfile(to_fd, from_fd, &offset, transfer_size);
    }

    fsync(from_fd);

    close(from_fd);
    close(to_fd);

    return(true);
}
