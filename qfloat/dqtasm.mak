qtst1.e20:  qtst1.obj jv.obj qjn.obj deinit.obj gamma.obj
	link20 qtst1 jv gamma qjn deinit dqlib dflib clib paslib

#	link20 qtst1 jv qjn deinit dql43 dflib clib paslib

qtst1.obj: qtst1.c qhead.h

jv.obj: jv.c mconf.h

qjn.obj: qjn.c qhead.h

gamma.obj: gamma.c mconf.h

deinit.obj: deinit.asm
	asm68k deinit.asm


