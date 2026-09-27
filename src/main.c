#include<stdio.h>
#include <string.h>

#include "cforge.h"
#include "hash.h"
#include "index.h"
#include "object.h"

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

	printf("Unknown command: %s\n", argv[1]);

}
