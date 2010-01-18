# Build and run test programs for qfloat.h C++ class.

CC = gcc
CFLAGS =  -O3 -Wall -g
CXXFLAGS = -O3 -Wall -g
#QLIB = libmq.a -lml -lmd
QLIB = libmq.a -lm

all: hello test1 test2 test3 test4 qtstq # clean

hello: hello.o
	$(CC) $(CFLAGS) -o hello hello.o -lstdc++
	hello > temp.tmp
	-diff temp.tmp hello.exp

test1: test1.o
	$(CC) $(CFLAGS) -o test1 test1.o $(QLIB)
	test1 > temp.tmp
	-diff temp.tmp test1.exp

test2: test2.o
	$(CC) $(CFLAGS) -o test2 test2.o $(QLIB)
	test2 > temp.tmp
	-diff temp.tmp test2.exp

test3: test3.o
	$(CC) $(CFLAGS) -o test3 test3.o $(QLIB)
	test3 > temp.tmp
	-diff temp.tmp test3.exp

test4: test4.o
	$(CC) $(CFLAGS) -o test4 test4.o $(QLIB)
	test4 > temp.tmp
	-diff temp.tmp test4.exp

qtstq: qtstq.o
	$(CC) $(CFLAGS) -o qtstq qtstq.o $(QLIB)
	qtstq > temp.tmp
	-diff temp.tmp qtstq.exp

clean:
	rm -f *.o
	rm -f hello
	rm -f test1
	rm -f test2
	rm -f test3
	rm -f test4
	rm -f qtstq
	rm -f temp.tmp
