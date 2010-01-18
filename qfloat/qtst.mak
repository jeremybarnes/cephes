CC = gcc
CFLAGS =  -O2 -Wall -g

qtst1: qtst1.o 
	$(CC) -o qtst1 qtst1.o -lmq -lmd
