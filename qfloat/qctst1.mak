CC = gcc
CFLAGS = -g -O -Wall
qctst: qctst1.o qcmplx.o clog.o
	$(CC) -o qctst1 qctst1.o qcmplx.o clog.o -lmq -lmd

qctst1.o: qctst1.c

qcmplx.o: qcmplx.c

