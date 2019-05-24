CC=gcc
CFLAGS=-g -Wall
TARGET=ht

all:
		$(CC) $(CFLAGS) utils.c prime.c hash_table.c main.c -lm -o $(TARGET)

clean:
		rm -f ht *.o a.out

