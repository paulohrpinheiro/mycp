#ifndef __CP_H__
#define __CP_H__

#include <stdbool.h>


bool naive_cp(const char*, const char*);
bool mmap_cp(const char*, const char*);
bool sendfile_cp(const char*, const char*);

#endif
