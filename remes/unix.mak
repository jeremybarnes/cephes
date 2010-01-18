# Unix make file for remes.exe
CC= gcc
INCS= remes.h
CFLAGS= -O2 -Wall
OBJS= remes.o remesa.o remese.o remesf.o remesg.o remesp.o \
remess.o remesw.o simq.o

remes: $(OBJS) $(INCS)
	$(CC) $(OBJS) -o remes -lm

