#include<stdio.h>
#include <string.h>
#include <dirent.h>

#include "cforge.h"
#include "hash.h"
#include "index.h"
#include "object.h"
#include "commit.h"

int main(int argc, char *argv[]) {
    
	if (argc < 2) {
		printf("Usage: cforge <command>\n");
		return 1;
	}

	if (strcmp(argv[1], "init") == 0) {
		return cforge_init();
	}

	if (strcmp(argv[1], "add") == 0) {

		if (argc < 3) {
			fprintf(stderr, "Usage: cforge add <file>\n");
			return 1;
		}

		unsigned long object_id;

		if (store_object(argv[2], &object_id) != 0) {
			return 1;
		}

		if (update_index(argv[2], object_id) != 0) {
			return 1;
		}

		printf("Added '%s' to staging area\n", argv[2]);

		return 0;
	}

	if (strcmp(argv[1], "status") == 0) {

		print_staged_files();

		DIR *directory = opendir(".");

		if (directory == NULL) {
			perror("cforge: failed to open directory");
			return 1;
		}

		struct dirent *entry;

		while ((entry = readdir(directory)) != NULL) {

			if (strcmp(entry->d_name, ".") == 0 ||
				strcmp(entry->d_name, "..") == 0 ||
				strcmp(entry->d_name ,".cforge") == 0) {
					continue;
			}

			if (!is_tracked(entry->d_name)) {
				printf("Untracked: %s\n", entry->d_name);
			}

			IndexEntry entries[MAX_INDEX_ENTRIES];
			int count;

			load_index(entries, &count);

			for (int i = 0; i < count; i++) {

				if (strcmp(entries[i].filepath, entry->d_name) == 0) {

					unsigned long current_hash = 
						hash_file(entry->d_name);

					if (current_hash != entries[i].object_id) {
						printf("Modified: %s\n", entry->d_name);
					}

					break;
				}
			}
		}

		closedir(directory);

		return 0;
	}

	if (strcmp(argv[1], "commit") == 0) {

    	if (argc < 3) {
        	fprintf(stderr, "Usage: cforge commit <message>\n");
        	return 1;
    	}

    	return create_commit(argv[2]);
	}

	if (strcmp(argv[1], "log") == 0) {
		return show_log();
	}

	printf("Unknown command: %s\n", argv[1]);

}
