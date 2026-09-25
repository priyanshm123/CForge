#include <stdio.h>
#include <sys/stat.h>

#include "cforge.h"

int cforge_init(void) {

	struct stat st;

	if (stat(".cforge", &st) == 0) {

		if (S_ISDIR(st.st_mode)) {
			printf("CForge repository already exists\n");
			return 0;
		}

		fprintf(stderr, "Error: .cforge exists but is not a directory\n");
		return 1;
	}

	if (mkdir(".cforge", 0755) != 0) {
		perror("cforge");
		return 1;
	}

	if (mkdir(".cforge/objects", 0755) != 0) {
		perror("cforge: failed to create objects");
		return 1;
	}

	if (mkdir(".cforge/refs", 0755) != 0) {
		perror("cforge: failed to create refs");
		return 1;
	}

	if (mkdir(".cforge/refs/heads", 0755) != 0) {
		perror("cforge: failed to create refs/heads");
		return 1;
	}

	if (mkdir(".cforge/refs/tags", 0755) != 0) {
		perror("cforge: failed to create refs/tags");
		return 1;
	}

	printf("Initialized empty CForge repository\n");

	FILE *head = fopen(".cforge/HEAD", "w");

	if (head == NULL) {
		perror("cforge: failed to create HEAD");
		return 1;
	}

	fprintf(head, "ref: refs/heads/main\n");

	fclose(head);

	FILE *index = fopen(".cforge/index", "w");

	if (index == NULL) {
		perror("cforge: failed to create index");
		return 1;
	}

	fclose(index);

	return 0;
}
