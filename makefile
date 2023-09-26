CC = gcc
CFLAGS = -Wall -g -c -lm -pthread -I .

all: ep1_main

ep1_main: ep1_main.o spend_time.o
	$(CC) -Wall -g ep1_main.o spend_time.o -o ep1_main -lm -pthread

ep1_main.o: ep1_main.c
	$(CC) ep1_main.c $(CFLAGS)

spend_time.o: spend_time.c
	$(CC) spend_time.c $(CFLAGS)