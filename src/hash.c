#include <stdio.h>
#include <stdlib.h>

#include "hash.h"

unsigned long hash_data(const unsigned char *data, size_t length) {
    unsigned long hash = 5381;

    for (size_t i = 0; i < length; i++) {
        hash = hash * 33 + data[i];
    }

    return hash;
}

unsigned long hash_file(const char *filepath) {
    FILE *file = fopen(filepath, "rb");

    if (file == NULL) {
        return 0;
    }

    fseek(file, 0, SEEK_END);

    long file_size = ftell(file);

    rewind(file);

    unsigned char *buffer = malloc(file_size);

    if (buffer == NULL && file_size > 0) {
        fclose(file);
        return 0;
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);

    fclose(file);

    unsigned long hash = hash_data(buffer, bytes_read);

    free(buffer);

    return hash;
}