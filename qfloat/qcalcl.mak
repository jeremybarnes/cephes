# MSDOS make file for qcalc.exe
CFLAGS=/c /AL

qcalc.obj: qcalc.c qhead.h qcalc.h mconf.h
	cl $(CFLAGS) /Od /DMOREFUNS=1 qcalc.c

qflt.obj: qflt.c qhead.h mconf.h
	cl $(CFLAGS) qflt.c

qfltb.obj: qfltb.c qhead.h mconf.h
	cl $(CFLAGS) qfltb.c

subml.obj: subml.asm qhead.h mconf.h qheadl.asm
	masm subml.asm;

mulrl.obj: mulrl.asm qhead.h mconf.h qheadl.asm
	masm mulrl.asm;

divnl.obj: divnl.asm qhead.h mconf.h qheadl.asm
	masm divnl.asm;

# Strictly rounded version
#qcalc.exe: qcalc.obj qlog.obj qexp.obj qpow.obj qtanh.obj qsqrta.obj \
#qfloor.obj qflt.obj qflta.obj mtherr.obj qconst.obj
#	link /NOE qcalc qsqrta qflt qflta mtherr,,,qlibl.lib;

# Non strictly rounded version, all C language
#qcalc.exe: qcalc.obj qlog.obj qexp.obj qpow.obj qtanh.obj qsqrt.obj \
#qfloor.obj qflt.obj qfltb.obj mtherr.obj qconst.obj
#	link /NOE qcalc qflt qfltb mtherr,,,qlibl.lib;

# Fast version with assembly language inner loops
qcalc.exe: qcalc.obj qflt.obj subml.obj mulrl.obj divnl.obj
	link /NOE /MAP /STACK:8192 qcalc qflt subml mulrl divnl mtherr,,,qlibl100.lib;

