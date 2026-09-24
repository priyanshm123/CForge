CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

TARGET = cforge

SRC = src/main.c src/cforge.c
OBJ = main.o cforge.o

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o main.o

cforge.o: src/cforge.c
	$(CC) $(CFLAGS) -c src/cforge.c -o cforge.o

clean:
	rm -f $(OBJ) $(TARGET)