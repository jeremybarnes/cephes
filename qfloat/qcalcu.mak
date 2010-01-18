CC= cc
CFLAGS= -O -g -Wall
OBJS= qcalc.o qflti.o qfltbi.o qsqrt.o
INCS= mconf.h qcalc.h qhead.h

qcalc: $(OBJS) $(INCS) libmq.a
	$(CC) $(OBJS) -o qcalc libmq.a -lmd
#	$(CC) $(OBJS) -o qcalc qfltb.o libmq.a -lmd
#	$(CC) $(OBJS) -o qcalc -lmq -lmd

#qfltb.o: qfltb.386
#	as -o qfltb.o qfltb.386
