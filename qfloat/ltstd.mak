CC = gcc
CFLAGS = -O2 -Wall -g
OBJS = 

ltstd:  ltstd.o $(OBJS)
	gcc -o ltstd ltstd.o $(OBJS) -lml -lmd
