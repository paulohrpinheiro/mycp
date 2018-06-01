#include <stdio.h>  /* fputs */
#include <stdlib.h> /* EXIT_* */
#include <string.h> /* strcmp */

#include "mycp.h"


int main(int argc, char *argv[]) {
    bool result=false;

    if(argc!=4) {
        fputs("Please, give-me (only) three arguments: method source destination", stderr);
        fputs("where 'method' is one of 'naive' or 'mmap'.", stderr);
        exit(EXIT_FAILURE);
    }

    if(0==strcmp("naive", argv[1])) {
        result = naive_cp(argv[2], argv[3]);
    }
    else if(0==strcmp("mmap", argv[1])) {
        result = mmap_cp(argv[2], argv[3]);
    }
    else if(0==strcmp("sendfile", argv[1])) {
        result = sendfile_cp(argv[2], argv[3]);
    }
    else {
        fputs("Unknown method.", stderr);
        result = false;
    }

    if(!result) {
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
