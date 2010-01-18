CC = cc
CFLAGS = -O
OBJS = qparanoi.o qtanh.o qexp.o qlog.o qpow.o qsqrta.o qfloor.o \
qflti.o qfltbi.o qconst.o mtherr.o
INCS = qhead.h mconf.h

qparanoi: $(OBJS) $(INCS)
	$(CC) -o qparanoi $(OBJS)

#qfltb.o: qfltb.386
#	as -o qfltb.o qfltb.386

qfltbi.o: qfltbi.386
	as -o qfltbi.o qfltbi.386

# Strictly rounded version
#qparanoi.exe: qparanoi.obj qlog.obj qexp.obj qpow.obj qtanh.obj qsqrta.obj \
#qfloor.obj qflt.obj qflta.obj mtherr.obj qconst.obj
#	link qparanoi qlog qexp qpow qsqrta qtanh qfloor qflt qflta mtherr qconst;

# Non strictly rounded version, all C language
#qparanoi.exe: qparanoi.obj qlog.obj qexp.obj qpow.obj qtanh.obj qsqrt.obj \
#qfloor.obj qflt.obj qfltb.obj mtherr.obj qconst.obj
#	link qparanoi qlog qexp qpow qsqrt qtanh qfloor qflt qfltb mtherr qconst;

# Fast version with assembly language inner loops
#qparanoi.exe: qparanoi.obj qlog.obj qexp.obj qpow.obj qtanh.obj qsqrt.obj \
#qfloor.obj qflt.obj subm.obj mulr.obj divn.obj mtherr.obj qconst.obj
#	link qparanoi qlog qexp qpow qsqrt qtanh qfloor qflt subm mulr divn mtherr qconst;

