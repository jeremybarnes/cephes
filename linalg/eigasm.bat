rem MSDOS batch file for eigenvector test program
rem fti.lib contains drand() random number generator.
cl /c blas.c
cl /c eigens.c
cl /c eigtst.c
cl eigtst.c eigens.obj blas.obj \lib\fti.lib
