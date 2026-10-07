#include <stdio.h>
#include <string.h>
#include "index.h"

#include "commit.h"

int load_index(IndexEntry entries[], int *count) {
    FILE *index = fopen(".cforge/index", "r");

    *count = 0;

    if (index == NULL) {
        return 0;
    }

    while (*count < MAX_INDEX_ENTRIES&&
        fscanf(
            index,
            "%255s %lu",
            entries[*count].filepath,
            &entries[*count].object_id
    ) == 2) {
        (*count)++;
    }

    fclose(index);

    return 0;
}

int update_index(const char *filepath, unsigned long object_id)
{
    IndexEntry entries[MAX_INDEX_ENTRIES];
    int count;

    if (load_index(entries, &count) != 0) {
        return 1;
    }

    for (int i = 0; i < count; i++) {

        if (strcmp(entries[i].filepath, filepath) == 0) {

            entries[i].object_id = object_id;

            FILE *index = fopen(".cforge/index", "w");

            if (index == NULL) {
                perror("cforge: failed to open index");
                return 1;
            }

            for (int j = 0; j < count; j++) {
                fprintf(
                    index,
                    "%s %lu\n",
                    entries[j].filepath,
                    entries[j].object_id
                );
            }

            fclose(index);

            return 0;
        }
    }

    if (count >= MAX_INDEX_ENTRIES) {
        fprintf(stderr, "cforge: index is full\n");
        return 1;
    }

    strcpy(entries[count].filepath, filepath);
    entries[count].object_id = object_id;
    count++;

    FILE *index = fopen(".cforge/index", "w");

    if (index == NULL) {
        perror("cforge: failed to open index");
        return 1;
    }

    for (int i = 0; i < count; i++) {
        fprintf(
            index,
            "%s %lu\n",
            entries[i].filepath,
            entries[i].object_id
        );
    }

    fclose(index);

    return 0;
}

int is_tracked(const char *filepath) {
    IndexEntry entries[MAX_INDEX_ENTRIES];
    int count;

    if (load_index(entries, &count) != 0) {
        return 0;
    }

    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].filepath, filepath) == 0) {
            return 1;
        }
    }

    return 0;
}

int print_staged_files(void)
{
    IndexEntry entries[MAX_INDEX_ENTRIES];
    int count;

    if (load_index(entries, &count) != 0) {
        return 1;
    }

    if (count == 0) {
        return 0;
    }

    printf("Changes to be committed:\n");

    for (int i = 0; i < count; i++) {
        printf("    %s\n", entries[i].filepath);
    }

    return 0;
}

int print_staged_files_against_head(void)
{
    IndexEntry index_entries[MAX_INDEX_ENTRIES];
    IndexEntry commit_entries[MAX_INDEX_ENTRIES];

    int index_count;
    int commit_count;

    if (load_index(index_entries, &index_count) != 0) {
        return 1;
    }

    unsigned long head_commit = get_head_commit();

    if (load_commit_files(
            head_commit,
            commit_entries,
            &commit_count
        ) != 0) {
        return 1;
    }

    int printed_header = 0;

    for (int i = 0; i < index_count; i++) {

        int found = 0;

        for (int j = 0; j < commit_count; j++) {

            if (strcmp(
                    index_entries[i].filepath,
                    commit_entries[j].filepath
                ) == 0) {

                found = 1;

                if (index_entries[i].object_id !=
                    commit_entries[j].object_id) {

                    if (!printed_header) {
                        printf("Changes to be committed:\n");
                        printed_header = 1;
                    }

                    printf(
                        "    %s\n",
                        index_entries[i].filepath
                    );
                }

                break;
            }
        }

        if (!found) {

            if (!printed_header) {
                printf("Changes to be committed:\n");
                printed_header = 1;
            }

            printf(
                "    %s\n",
                index_entries[i].filepath
            );
        }
    }

    return 0;
}

int index_matches_commit(
    IndexEntry index_entries[],
    int index_count,
    IndexEntry commit_entries[],
    int commit_count
) {
    if (index_count != commit_count) {
        return 0;
    }

    for (int i = 0; i < index_count; i++) {
        int found = 0;

        for (int j = 0; j < commit_count; j++) {
            if (strcmp(
                index_entries[i].filepath,
                commit_entries[j].filepath
            ) == 0) {
                found = 1;

                if (index_entries[i].object_id != commit_entries[j].object_id) {
                    return 0;
                }

                break;
            }
        }
        if (!found) {
            return 0;
        }
    }

    return 1;
}