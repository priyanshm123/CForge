#include <stdio.h>
#include <stdlib.h>

#include "commit.h"
#include "index.h"
#include "hash.h"

static unsigned long get_current_commit(void)
{
    FILE *file = fopen(".cforge/refs/heads/main", "r");

    if (file == NULL) {
        return 0;
    }

    unsigned long commit_id;

    if (fscanf(file, "%lu", &commit_id) != 1) {
        fclose(file);
        return 0;
    }

    fclose(file);

    return commit_id;
}

int create_commit(const char *message)
{
    IndexEntry entries[MAX_INDEX_ENTRIES];
    int count;

    if (load_index(entries, &count) != 0) {
        return 1;
    }

    if (count == 0) {
        fprintf(stderr, "cforge: nothing to commit\n");
        return 1;
    }

    unsigned long parent = get_current_commit();

    char commit_data[8192];
    int offset = 0;

    offset += snprintf(
        commit_data + offset,
        sizeof(commit_data) - offset,
        "parent: %lu\n",
        parent
    );

    offset += snprintf(
        commit_data + offset,
        sizeof(commit_data) - offset,
        "message: %s\n\n",
        message
    );

    for (int i = 0; i < count; i++) {

        offset += snprintf(
            commit_data + offset,
            sizeof(commit_data) - offset,
            "%s %lu\n",
            entries[i].filepath,
            entries[i].object_id
        );
    }

    unsigned long commit_id = hash_data(
        (unsigned char *)commit_data,
        offset
    );

    char object_path[256];

    snprintf(
        object_path,
        sizeof(object_path),
        ".cforge/objects/%lu",
        commit_id
    );

    FILE *object = fopen(object_path, "wb");

    if (object == NULL) {
        perror("cforge: failed to create commit object");
        return 1;
    }

    if (fwrite(commit_data, 1, offset, object) != (size_t)offset) {
        fprintf(stderr, "cforge: failed to write commit object\n");
        fclose(object);
        return 1;
    }

    fclose(object);

    FILE *head = fopen(".cforge/refs/heads/main", "w");

    if (head == NULL) {
        perror("cforge: failed to update HEAD");
        return 1;
    }

    fprintf(head, "%lu\n", commit_id);

    fclose(head);

    printf("Commit ID: %lu\n", commit_id);
    printf("%s", commit_data);

    return 0;
}