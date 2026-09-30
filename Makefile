CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

TARGET = cforge

OBJ = main.o cforge.o hash.o object.o index.o commit.o
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o main.o

cforge.o: src/cforge.c
	$(CC) $(CFLAGS) -c src/cforge.c -o cforge.o

hash.o: src/hash.c
	$(CC) $(CFLAGS) -c src/hash.c -o hash.o

object.o: src/object.c
	$(CC) $(CFLAGS) -c src/object.c -o object.o

index.o: src/index.c
	$(CC) $(CFLAGS) -c src/index.c -o index.o

commit.o: src/commit.c
	$(CC) $(CFLAGS) -c src/commit.c -o commit.o

clean:
	rm -f $(OBJ) $(TARGET)