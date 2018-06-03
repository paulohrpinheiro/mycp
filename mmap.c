#include <fcntl.h>    /* open */
#include <stdio.h>    /* printf */
#include <unistd.h>   /* close */
#include <stdbool.h>  /* true, false */
#include <string.h>   /* memcpy */
#include <sys/mman.h> /* mmap */
#include <sys/stat.h> /* struct stat */

#include "mycp.h"


bool mmap_cp(const char *from_filename, const char *to_filename) {
    int from_fd, to_fd;
    off_t filesize;
    void *source, *target;
    struct stat status;
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

    lseek(to_fd, filesize - 1, SEEK_SET);
    write(to_fd, "", 1);

    source = mmap(NULL, filesize, PROT_READ, MAP_SHARED, from_fd, 0);
    if(MAP_FAILED==source) {
        perror("mmap source");
        goto close_all;
    }

    target = mmap(NULL, filesize, PROT_READ|PROT_WRITE, MAP_SHARED, to_fd, 0);
    if(MAP_FAILED==target) {
        perror("mmap target");
        goto munmap_source;
    }

    memcpy(target, source, filesize);

    if(-1==fsync(from_fd)) {
        perror("fsync()");
        goto clean_all;
    }

    response = true;

clean_all:
    if(-1==munmap(target, filesize)) {
        perror("munmap target");
    }

munmap_source:
    if(-1==munmap(source, filesize)) {
        perror("munmap source");
    }

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
