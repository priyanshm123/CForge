#ifndef INDEX_H
#define INDEX_H

#define MAX_INDEX_ENTRIES 1000
#define MAX_PATH_LENGTH 256

typedef struct {
    char filepath[MAX_PATH_LENGTH];
    unsigned long object_id;
} IndexEntry;

int load_index(IndexEntry entries[], int *count);
int update_index(const char *filepath, unsigned long object_id);
int is_tracked(const char *filepath);
int print_staged_files(void);
int print_staged_files_against_head(void);
int index_matches_commit(
    IndexEntry index_entries[],
    int index_count,
    IndexEntry commit_entries[],
    int commit_count
);

#endif
