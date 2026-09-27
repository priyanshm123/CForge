#include <stdio.h>
#include "index.h"

int update_index(const char *filepath, unsigned long object_id) {
    FILE *index = fopen(".cforge/index", "a");

    if (index == NULL) {
        perror("cforge: failed to open index");
        return 1;
    }

    if (fprintf(index, "%s %lu\n", filepath, object_id) < 0) {
        fprintf(stderr, "cforge: failed to update index\n");
        fclose(index);
        return 1;
    }

    fclose(index);

    return 0;
}