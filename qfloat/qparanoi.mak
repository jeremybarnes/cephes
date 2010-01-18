# MSDOS make file for qcalc.exe
CFLAGS=/c

qtanh.obj: qtanh.c qhead.h mconf.h
	cl $(CFLAGS) qtanh.c

#qcalc.obj: qcalc.c qhead.h qcalc.h mconf.h
#	cl $(CFLAGS) qcalc.c

qexp.obj: qexp.c qhead.h mconf.h
	cl $(CFLAGS) qexp.c

qparanoi.obj: qparanoi.c qhead.h mconf.h
	cl $(CFLAGS) qparanoi.c

qlog.obj: qlog.c qhead.h mconf.h
	cl $(CFLAGS) qlog.c

qpow.obj: qpow.c qhead.h mconf.h
	cl $(CFLAGS) qpow.c

qsqrt.obj: qsqrt.c qhead.h mconf.h
	cl $(CFLAGS) qsqrt.c

qsqrta.obj: qsqrta.c qhead.h mconf.h
	cl $(CFLAGS) qsqrta.c

qfloor.obj: qfloor.c qhead.h mconf.h
	cl $(CFLAGS) qfloor.c

qflt.obj: qflt.c qhead.h mconf.h
	cl $(CFLAGS) qflt.c

qflta.obj: qflta.c qhead.h mconf.h
	cl $(CFLAGS) qflta.c

qfltb.obj: qfltb.c qhead.h mconf.h
	cl $(CFLAGS) /Ox qfltb.c

subm.obj: subm.asm qhead.h mconf.h qhead.asm
	masm subm.asm;

mulr.obj: mulr.asm qhead.h mconf.h qhead.asm
	masm mulr.asm;

divn.obj: divn.asm qhead.h mconf.h qhead.asm
	masm divn.asm;

qconst.obj: qconst.c qhead.h mconf.h
	cl $(CFLAGS) qconst.c

mtherr.obj: mtherr.c qhead.h mconf.h
	cl $(CFLAGS) mtherr.c

# Strictly rounded version
#qparanoi.exe: qparanoi.obj qlog.obj qexp.obj qpow.obj qtanh.obj qsqrta.obj \
#qfloor.obj qflt.obj qflta.obj mtherr.obj qconst.obj
#	link qparanoi qlog qexp qpow qsqrta qtanh qfloor qflt qflta mtherr qconst;

# Non strictly rounded version, all C language
qparanoi.exe: qparanoi.obj qlog.obj qexp.obj qpow.obj qtanh.obj qsqrt.obj \
qfloor.obj qflt.obj qfltb.obj mtherr.obj qconst.obj
	link qparanoi qlog qexp qpow qsqrt qtanh qfloor qflt qfltb mtherr qconst;

# Fast version with assembly language inner loops
#qparanoi.exe: qparanoi.obj qlog.obj qexp.obj qpow.obj qtanh.obj qsqrt.obj \
#qfloor.obj qflt.obj subm.obj mulr.obj divn.obj mtherr.obj qconst.obj
#	link qparanoi qlog qexp qpow qsqrt qtanh qfloor qflt subm mulr divn mtherr qconst;

