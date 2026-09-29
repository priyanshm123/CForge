#ifndef HASH_H
#define HASH_H

#include <stddef.h>

unsigned long hash_data(const unsigned char *data, size_t length);
unsigned long hash_file(const char *filepath);

#endif