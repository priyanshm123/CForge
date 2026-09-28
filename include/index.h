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

#endif
