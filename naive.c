#include <stdio.h> /* perror */
#include <stdlib.h> /* EXIT_*, size_t, ssize_t */
#include <unistd.h> /* read, write, close */
#include <stdbool.h> /* true, false */
#include <sys/stat.h> /* struct stat */

#include "mycp.h"
#include "util.h"

bool naive_cp(const char *from_filename, const char *to_filename) {
    int from_fd, to_fd;
    size_t block_size;
    ssize_t readed, writed;
    char *buffer;

    block_size = best_block_size();

    from_fd = open_from(from_filename);
    to_fd = open_to(to_filename);

    buffer = (char *)malloc(block_size);
    if(NULL==buffer) {
        perror("buffer");
        return(false);
    }

    while(0 != (readed = read(from_fd, buffer, block_size))) {
        if(-1==readed) {
            perror("\nCannot read 'from' file");
            free(buffer);
            return(false);
        }

        writed = write(to_fd, buffer, readed);

        if(-1==writed) {
            perror("\nCannot write 'to' file");
            free(buffer);
            return(false);
        }

        if(0==readed) {
            break;
        }
    }

    free(buffer);

    fsync(from_fd);

    close(from_fd);
    close(to_fd);

    return(true);
}
