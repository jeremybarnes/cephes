#
# MSDOS makefile for ellf.exe
#
ellf.obj: ellf.c
	cl /c ellf.c

ellik.obj: ellik.c
	cl /c ellik.c

ellpj.obj: ellpj.c
	cl /c ellpj.c

ellpk.obj: ellpk.c mconf.h
	cl /c ellpk.c

cmplx.obj: cmplx.c mconf.h
	cl /c cmplx.c

polevl.obj: polevl.c mconf.h
	cl /c polevl.c

mtherr.obj: mtherr.c mconf.h
	cl /c mtherr.c

const.obj: const.c mconf.h
	cl /c const.c

ellf.exe: ellf.obj ellik.obj ellpj.obj ellpk.obj cmplx.obj const.obj polevl.obj mtherr.obj
	link ellf ellik ellpj ellpk cmplx polevl const mtherr;
