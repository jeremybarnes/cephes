
qtst1.obj: qtst1.c qhead.h
	cl /c qtst1.c qhead.h

hyp2f1.obj: hyp2f1.c
	cl /c hyp2f1.c

qhy2f1.obj: qhy2f1.c qhead.h
	cl /c qhy2f1.c

qgamma.obj: qgamma.c qhead.h
	cl /c qgamma.c

#deinit.obj: deinit.asm
#	load asm68k deinit.asm

ieinit.obj: ieinit.asm
	masm ieinit.asm;

qtst1.exe:  qtst1.obj hyp2f1.obj qhy2f1.obj ieinit.obj
	link /MAP /NOE qtst1 hyp2f1 qhy2f1 ieinit,,,\lib\qlib.lib \lib\fti.lib;

#	link /MAP /NOE qtst1 hyp2f1 qgamma qhy2f1 ieinit,,,\lib\qlib100.lib \lib\fti.lib;
