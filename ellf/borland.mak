#
# Borland C++ IDE generated makefile
#
.AUTODEPEND


#
# Borland C++ tools
#
IMPLIB  = Implib
BCC32   = Bcc32 +BccW32.cfg 
TLINK32 = TLink32
TLIB    = TLib
BRC32   = Brc32
TASM32  = Tasm32
#
# IDE macros
#


#
# Options
#
IDE_LFLAGS32 =  -LD:\BC45\LIB
LLATC32_ellfdexe =  -Tpe -ap -c
RLATC32_ellfdexe =  -w32
BLATC32_ellfdexe = 
CNIEAT_ellfdexe = -ID:\BC45\INCLUDE -D
LNIEAT_ellfdexe = -x
LEAT_ellfdexe = $(LLATC32_ellfdexe)
REAT_ellfdexe = $(RLATC32_ellfdexe)
BEAT_ellfdexe = $(BLATC32_ellfdexe)

#
# Dependency List
#
Dep_proj0000 = \
   ellf.exe

proj0000 : BccW32.cfg $(Dep_proj0000)
  echo MakeNode 

Dep_ellfdexe = \
   cmplx.obj\
   ellf.obj\
   ellie.obj\
   ellik.obj\
   ellpe.obj\
   ellpj.obj\
   ellpk.obj\
   mtherr.obj\
   polevl.obj\
   const.obj

ellf.exe : $(Dep_ellfdexe)
  $(TLINK32) @&&|
 /v $(IDE_LFLAGS32) $(LEAT_ellfdexe) $(LNIEAT_ellfdexe) +
D:\BC45\LIB\c0x32.obj+
cmplx.obj+
ellf.obj+
ellie.obj+
ellik.obj+
ellpe.obj+
ellpj.obj+
ellpk.obj+
mtherr.obj+
polevl.obj+
const.obj
$<,$*
D:\BC45\LIB\import32.lib+
D:\BC45\LIB\cw32.lib

|

cmplx.obj :  cmplx.c
  $(BCC32) -P- -c @&&|
 $(CEAT_ellfdexe) $(CNIEAT_ellfdexe) -o$@ cmplx.c
|

ellf.obj :  ellf.c
  $(BCC32) -P- -c @&&|
 $(CEAT_ellfdexe) $(CNIEAT_ellfdexe) -o$@ ellf.c
|

ellie.obj :  ellie.c
  $(BCC32) -P- -c @&&|
 $(CEAT_ellfdexe) $(CNIEAT_ellfdexe) -o$@ ellie.c
|

ellik.obj :  ellik.c
  $(BCC32) -P- -c @&&|
 $(CEAT_ellfdexe) $(CNIEAT_ellfdexe) -o$@ ellik.c
|

ellpe.obj :  ellpe.c
  $(BCC32) -P- -c @&&|
 $(CEAT_ellfdexe) $(CNIEAT_ellfdexe) -o$@ ellpe.c
|

ellpj.obj :  ellpj.c
  $(BCC32) -P- -c @&&|
 $(CEAT_ellfdexe) $(CNIEAT_ellfdexe) -o$@ ellpj.c
|

ellpk.obj :  ellpk.c
  $(BCC32) -P- -c @&&|
 $(CEAT_ellfdexe) $(CNIEAT_ellfdexe) -o$@ ellpk.c
|

mtherr.obj :  mtherr.c
  $(BCC32) -P- -c @&&|
 $(CEAT_ellfdexe) $(CNIEAT_ellfdexe) -o$@ mtherr.c
|

polevl.obj :  polevl.c
  $(BCC32) -P- -c @&&|
 $(CEAT_ellfdexe) $(CNIEAT_ellfdexe) -o$@ polevl.c
|

const.obj :  const.c
  $(BCC32) -P- -c @&&|
 $(CEAT_ellfdexe) $(CNIEAT_ellfdexe) -o$@ const.c
|

# Compiler configuration file
BccW32.cfg : 
   Copy &&|
-R
-v
-vi
-H
-H=proj0000.csm
-WC
-A
| $@


