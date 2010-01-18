#
# Borland C++ IDE generated makefile
#
.AUTODEPEND


#
# Borland C++ tools
#
IMPLIB  = Implib
BCCDOS  = Bcc +BccDos.cfg 
TLINK   = TLink
TLIB    = TLib
TASM    = Tasm
#
# IDE macros
#


#
# Options
#
IDE_LFLAGSDOS =  -LC:\BC45\LIB
IDE_BFLAGS = 
LLATDOS_ldoubledlib =  -c -Tde
RLATDOS_ldoubledlib = 
BLATDOS_ldoubledlib = 
CNIEAT_ldoubledlib = -IC:\BC45\INCLUDE;C:\MATH\LDOUBLE -D
LNIEAT_ldoubledlib = -x
LEAT_ldoubledlib = $(LLATDOS_ldoubledlib)
REAT_ldoubledlib = $(RLATDOS_ldoubledlib)
BEAT_ldoubledlib = $(BLATDOS_ldoubledlib)

#
# Dependency List
#
Dep_ldouble = \
   ldouble.lib

ldouble : BccDos.cfg $(Dep_ldouble)
  echo MakeNode 

Dep_ldoubledlib = \
   setprbor.obj\
   powl.obj\
   sinhl.obj\
   sinl.obj\
   stdtrl.obj\
   tanhl.obj\
   tanl.obj\
   unityl.obj\
   ynl.obj\
   mtherr.obj\
   nbdtrl.obj\
   ndtril.obj\
   ndtrl.obj\
   pdtrl.obj\
   polevll.obj\
   powil.obj\
   isnanl.obj\
   j0l.obj\
   j1l.obj\
   jnl.obj\
   ldrand.obj\
   log10l.obj\
   log2l.obj\
   logl.obj\
   floorl.obj\
   gammal.obj\
   gdtrl.obj\
   gelsl.obj\
   ieee.obj\
   igamil.obj\
   igaml.obj\
   incbetl.obj\
   incbil.obj\
   coshl.obj\
   econst.obj\
   elliel.obj\
   ellikl.obj\
   ellpel.obj\
   ellpjl.obj\
   ellpkl.obj\
   exp10l.obj\
   exp2l.obj\
   expl.obj\
   expm1l.obj\
   fdtrl.obj\
   acoshl.obj\
   arcdotl.obj\
   asinhl.obj\
   asinl.obj\
   atanhl.obj\
   atanl.obj\
   bdtrl.obj\
   btdtrl.obj\
   cbrtl.obj\
   chdtrl.obj\
   clogl.obj\
   cmplxl.obj

ldouble.lib : $(Dep_ldoubledlib)
  $(TLIB) $< $(IDE_BFLAGS) $(BEAT_ldoubledlib) @&&|
 -+setprbor.obj &
-+powl.obj &
-+sinhl.obj &
-+sinl.obj &
-+stdtrl.obj &
-+tanhl.obj &
-+tanl.obj &
-+unityl.obj &
-+ynl.obj &
-+mtherr.obj &
-+nbdtrl.obj &
-+ndtril.obj &
-+ndtrl.obj &
-+pdtrl.obj &
-+polevll.obj &
-+powil.obj &
-+isnanl.obj &
-+j0l.obj &
-+j1l.obj &
-+jnl.obj &
-+ldrand.obj &
-+log10l.obj &
-+log2l.obj &
-+logl.obj &
-+floorl.obj &
-+gammal.obj &
-+gdtrl.obj &
-+gelsl.obj &
-+ieee.obj &
-+igamil.obj &
-+igaml.obj &
-+incbetl.obj &
-+incbil.obj &
-+coshl.obj &
-+econst.obj &
-+elliel.obj &
-+ellikl.obj &
-+ellpel.obj &
-+ellpjl.obj &
-+ellpkl.obj &
-+exp10l.obj &
-+exp2l.obj &
-+expl.obj &
-+expm1l.obj &
-+fdtrl.obj &
-+acoshl.obj &
-+arcdotl.obj &
-+asinhl.obj &
-+asinl.obj &
-+atanhl.obj &
-+atanl.obj &
-+bdtrl.obj &
-+btdtrl.obj &
-+cbrtl.obj &
-+chdtrl.obj &
-+clogl.obj &
-+cmplxl.obj
|

setprbor.obj :  ldouble\setprbor.asm
  $(TASM) @&&|
 /ml C:\MATH\ldouble\setprbor.asm ,setprbor.obj
|

powl.obj :  ldouble\powl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\powl.c
|

sinhl.obj :  ldouble\sinhl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\sinhl.c
|

sinl.obj :  ldouble\sinl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\sinl.c
|

stdtrl.obj :  ldouble\stdtrl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\stdtrl.c
|

tanhl.obj :  ldouble\tanhl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\tanhl.c
|

tanl.obj :  ldouble\tanl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\tanl.c
|

unityl.obj :  ldouble\unityl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\unityl.c
|

ynl.obj :  ldouble\ynl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\ynl.c
|

mtherr.obj :  ldouble\mtherr.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\mtherr.c
|

nbdtrl.obj :  ldouble\nbdtrl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\nbdtrl.c
|

ndtril.obj :  ldouble\ndtril.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\ndtril.c
|

ndtrl.obj :  ldouble\ndtrl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\ndtrl.c
|

pdtrl.obj :  ldouble\pdtrl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\pdtrl.c
|

polevll.obj :  ldouble\polevll.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\polevll.c
|

powil.obj :  ldouble\powil.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\powil.c
|

isnanl.obj :  ldouble\isnanl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\isnanl.c
|

j0l.obj :  ldouble\j0l.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\j0l.c
|

j1l.obj :  ldouble\j1l.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\j1l.c
|

jnl.obj :  ldouble\jnl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\jnl.c
|

ldrand.obj :  ldouble\ldrand.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\ldrand.c
|

log10l.obj :  ldouble\log10l.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\log10l.c
|

log2l.obj :  ldouble\log2l.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\log2l.c
|

logl.obj :  ldouble\logl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\logl.c
|

floorl.obj :  ldouble\floorl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\floorl.c
|

gammal.obj :  ldouble\gammal.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\gammal.c
|

gdtrl.obj :  ldouble\gdtrl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\gdtrl.c
|

gelsl.obj :  ldouble\gelsl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\gelsl.c
|

ieee.obj :  ldouble\ieee.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\ieee.c
|

igamil.obj :  ldouble\igamil.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\igamil.c
|

igaml.obj :  ldouble\igaml.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\igaml.c
|

incbetl.obj :  ldouble\incbetl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\incbetl.c
|

incbil.obj :  ldouble\incbil.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\incbil.c
|

coshl.obj :  ldouble\coshl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\coshl.c
|

econst.obj :  ldouble\econst.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\econst.c
|

elliel.obj :  ldouble\elliel.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\elliel.c
|

ellikl.obj :  ldouble\ellikl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\ellikl.c
|

ellpel.obj :  ldouble\ellpel.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\ellpel.c
|

ellpjl.obj :  ldouble\ellpjl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\ellpjl.c
|

ellpkl.obj :  ldouble\ellpkl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\ellpkl.c
|

exp10l.obj :  ldouble\exp10l.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\exp10l.c
|

exp2l.obj :  ldouble\exp2l.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\exp2l.c
|

expl.obj :  ldouble\expl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\expl.c
|

expm1l.obj :  ldouble\expm1l.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\expm1l.c
|

fdtrl.obj :  ldouble\fdtrl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\fdtrl.c
|

acoshl.obj :  ldouble\acoshl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\acoshl.c
|

arcdotl.obj :  ldouble\arcdotl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\arcdotl.c
|

asinhl.obj :  ldouble\asinhl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\asinhl.c
|

asinl.obj :  ldouble\asinl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\asinl.c
|

atanhl.obj :  ldouble\atanhl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\atanhl.c
|

atanl.obj :  ldouble\atanl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\atanl.c
|

## The following section is generated by duplicate target bdtrl.obj.

bdtrl.obj :  ldouble\bdtrl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\bdtrl.c
|

## The above section is generated by duplicate target bdtrl.obj.

btdtrl.obj :  ldouble\btdtrl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\btdtrl.c
|

cbrtl.obj :  ldouble\cbrtl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\cbrtl.c
|

chdtrl.obj :  ldouble\chdtrl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\chdtrl.c
|

clogl.obj :  ldouble\clogl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\clogl.c
|

cmplxl.obj :  ldouble\cmplxl.c
  $(BCCDOS) -P- -c @&&|
 $(CEAT_ldoubledlib) $(CNIEAT_ldoubledlib) -o$@ ldouble\cmplxl.c
|

# Compiler configuration file
BccDos.cfg : 
   Copy &&|
-W-
-R
-v
-vi
-H
-H=ldouble.csm
-ml
-f
| $@


