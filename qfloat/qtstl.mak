CC = gcc
CFLAGS =  -O2 -Wall -g
OBJS = 

qtstl: qtstl.o $(OBJS)
	$(CC) $(CFLAGS) -o qtstl qtstl.o $(OBJS) -lmq -lml -lmd


