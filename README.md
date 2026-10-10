# CForge

CForge is a small, local version control system written from scratch in C. It stores file snapshots as content-hashed objects and groups staged files into commits linked by parent commit IDs. The project is intended as an educational exploration of version-control internals, not as a replacement for Git.

## Goals

- Demonstrate the basics of content-addressed object storage, an index (staging area), and a commit history.
- Provide a simple command-line workflow for initializing a repository, staging files, checking status, creating commits, and viewing history.
- Practice C programming concepts including file I/O, data structures, hashing, and persistent storage.

## Code architecture

- `src/main.c` implements command dispatch and the `init`, `add`, `status`, `commit`, and `log` commands.
- `src/cforge.c` initializes the repository metadata and directory structure.
- `src/object.c` reads files and stores their contents under a hash-derived object ID.
- `src/hash.c` implements the hash function used for file contents and commit data.
- `src/index.c` reads and updates the staging index and compares staged entries with the current commit.
- `src/commit.c` creates commit objects, updates the `main` branch reference, and displays commit history.
- `include/` contains the public declarations shared by the source files.

Repository data is stored in `.cforge/` in the current working directory. The `objects/` directory contains file and commit objects; `index` records staged file paths and object IDs; `refs/heads/main` points to the latest commit. `HEAD` identifies the main branch.

## Requirements

- GCC or another C compiler compatible with the Makefile's C11 flags
- GNU Make

## Compilation

From the project directory, build the executable with:

```sh
make
```

This creates the `cforge` executable in the project directory. To remove the executable and intermediate object files:

```sh
make clean
```

## How to run

Run CForge from the directory you want to track. Initialize that directory, then stage files and create commits:

```sh
/path/to/CForge/cforge init
/path/to/CForge/cforge add README.md
/path/to/CForge/cforge status
/path/to/CForge/cforge commit "Initial snapshot"
/path/to/CForge/cforge log
```

Replace `/path/to/CForge` with the location of the compiled executable. If CForge was built in the directory you want to track, use `./cforge` instead. For example, when working in the CForge project directory:

```sh
make
./cforge init
./cforge add README.md
./cforge status
./cforge commit "Initial snapshot"
./cforge log
```

Available commands:

| Command | Description |
| --- | --- |
| `cforge init` | Initialize a repository in the current directory. |
| `cforge add <file>` | Store the file's current contents and stage it. |
| `cforge status` | Show staged changes, modified tracked files, and untracked directory entries. |
| `cforge commit <message>` | Create a commit from the current index. |
| `cforge log` | Display commits starting at the current `main` tip. |

## Current scope

CForge currently supports the commands above. Features such as removing files, branching, merging, and restoring previous versions are not implemented yet. The implementation is intentionally small and does not provide Git's full feature set or robustness.
