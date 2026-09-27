#include "hash.h"

unsigned long hash_data(const unsigned char *data, size_t length) {
    unsigned long hash = 5381;

    for (size_t i = 0; i < length; i++) {
        hash = hash * 33 + data[i];
    }

    return hash;
}