CC = gcc
CFLAGS =  -O3 -Wall -g
CXXFLAGS = -O3 -Wall -g
OBJS = qremain.o qrand.o

qtstq: qtstq.o $(OBJS)
	$(CC) $(CFLAGS) -o qtstq qtstq.o $(OBJS) libmq.a -lml -lmd

qtstq.o: qtstq.cc qfloat.h
