# Unix makefile for ieetst
# Define computer type and/or endianness in mconf.h.
#
CC = gcc
CFLAGS= -O
INCS= mconf.h ehead.h
OBJS = ieee.o econst.o eexp.o elog.o epow.o etanh.o etodec.o mtherr.o
LDFLAGS =
LIBS = 

all: ieetst

ieetst: ieetst.o $(OBJS) drand.o $(INCS)
	$(CC) $(LDFLAGS) -o ieetst ieetst.o $(OBJS) drand.o $(LIBS)

ieee.o: ieee.c $(INCS)
	$(CC) $(CFLAGS) -c ieee.c

econst.o: econst.c $(INCS)
	$(CC) $(CFLAGS) -c econst.c

elog.o: elog.c $(INCS)
	$(CC) $(CFLAGS) -c elog.c

eexp.o: eexp.c $(INCS)
	$(CC) $(CFLAGS) -c eexp.c

etanh.o: etanh.c $(INCS)
	$(CC) $(CFLAGS) -c etanh.c

epow.o: epow.c $(INCS)
	$(CC) $(CFLAGS) -c epow.c

mtherr.o: mtherr.c $(INCS)
	$(CC) $(CFLAGS) -c mtherr.c

ieetst.o: ieetst.c $(INCS)
	$(CC) $(CFLAGS) -c ieetst.c

drand.o: drand.c $(INCS)
	$(CC) $(CFLAGS) -c drand.c

etodec.o: etodec.c $(INCS)
	$(CC) $(CFLAGS) -c etodec.c

clean:
	rm -f *.o
	rm -f ieetst
