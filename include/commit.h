#ifndef COMMIT_H
#define COMMIT_H

#include "index.h"

int create_commit(const char *message);
int show_log(void);
int load_commit_files(
    unsigned long commit_id,
    IndexEntry entries[],
    int *count
);
unsigned long get_head_commit(void);

#endif