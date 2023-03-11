SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra -g -Wstrict-prototypes -gdwarf-4 
LFLAGS = $(shell pkg-config --libs gmp)

all: keygen encrypt decrypt
	
keygen: keygen.o
	$(CC) -o keygen -lm keygen.o $(CFLAGS) -lgmp
	
keygen.o: keygen.c  
	$(CC) $(CFLAGS) -c keygen.c

encrypt: encrypt.o
	$(CC) -o encrypt -lm encrypt.o $(CFLAGS) -lgmp
	
encrypt.o: encrypt.c  
	$(CC) $(CFLAGS) -c encrypt.c
	
decrypt: decrypt.o
	$(CC) -o decrypt -lm decrypt.o $(CFLAGS) -lgmp
	
decrypt.o: decrypt.c  
	$(CC) $(CFLAGS) -c decrypt.c

clean:
	rm -f *.o encrypt decrypt keygen

format:
	clang-format -i -style=file *.[ch]

