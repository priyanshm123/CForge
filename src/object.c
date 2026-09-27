#include <stdio.h>
#include <stdlib.h>

#include "object.h"
#include "hash.h"

int store_object(const char *filepath, unsigned long *object_id) {
    FILE *file = fopen(filepath, "rb");

    if (file == NULL) {
        perror("cforge: failed to open file");
        return 1;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("cforge: failed to seek file");
        fclose(file);
        return 1;
    }

    long file_size = ftell(file);

    if (file_size < 0) {
        perror("cforge: failed to determine file size");
        fclose(file);
        return 1;
    }

    rewind(file);

    unsigned char *buffer = malloc(file_size);

    if (buffer == NULL && file_size > 0) {
        fprintf(stderr, "cforge: memory allocation failed\n");
        fclose(file);
        return 1;
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);

    if (bytes_read != (size_t)file_size) {
        fprintf(stderr, "cforge: failed to read file\n");
        free(buffer);
        fclose(file);
        return 1;
    }

    fclose(file);

    *object_id = hash_data(buffer, bytes_read);

    char object_name[32];

    snprintf(object_name, sizeof(object_name), "%lu", *object_id);

    char object_path[256];

    snprintf(
        object_path,
        sizeof(object_path),
        ".cforge/objects/%s",
        object_name
    );

    FILE *object = fopen(object_path, "wb");

    if (object == NULL) {
        perror("cforge: failed to create object");
        free(buffer);
        return 1;
    }

    size_t bytes_written = fwrite(buffer, 1, bytes_read, object);

    if (bytes_written != bytes_read) {
        fprintf(stderr, "cforge: failed to write object\n");
        fclose(object);
        free(buffer);
        return 1;
    }

    fclose(object);

    printf("Stored object: %s\n", object_name);

    free(buffer);

    return 0;
}