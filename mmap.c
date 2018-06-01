#include <stdio.h>    /* printf */
#include <unistd.h>   /* close */
#include <stdbool.h>  /* true, false */
#include <string.h>   /* memcpy */
#include <sys/mman.h> /* mmap */
#include <sys/stat.h> /* struct stat */

#include "mycp.h"
#include "util.h"


bool mmap_cp(const char *from_filename, const char *to_filename) {
    int from_fd, to_fd;
    off_t filesize;
    void *source, *target;
    struct stat statbuf;

    from_fd = open_from(from_filename);
    to_fd = open_to(to_filename);

    fstat(from_fd, &statbuf) ;
	filesize = statbuf.st_size;

    lseek(to_fd, filesize - 1, SEEK_SET);
    write(to_fd, "", 1);

    source = mmap(NULL, filesize, PROT_READ, MAP_SHARED, from_fd, 0);
    target = mmap(NULL, filesize, PROT_READ|PROT_WRITE, MAP_SHARED, to_fd, 0);

    memcpy(target, source, filesize);

    munmap(source, filesize);
    munmap(target, filesize);

    fsync(to_fd);

    close(from_fd);
    close(to_fd);

    return(true);
}
