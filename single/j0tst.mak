

j0tst: j0tst.o j0f.o libmf.a
	cc -o j0tst j0tst.o j0f.o libmf.a

j0f.o: j0f.c mconf.h
j0tst.o: j0tst.c mconf.h
