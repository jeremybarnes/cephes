#
# Unix makefile for digital filter calculator
#

CC= gcc
CFLAGS= -O
OBJS= ellf.o ellik.o ellpj.o ellpk.o cmplx.o polevl.o const.o mtherr.o
INCS= mconf.h

ellf: $(OBJS) $(INCS)
	$(CC) $(CFLAGS) -o ellf $(OBJS) -lm
