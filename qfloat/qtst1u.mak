CC = gcc
CFLAGS = -O -Wall -g

qtst1:  qtst1.o qincb.o incbet.o beta.o incbi.o incbil.o qincbi.o incbetl.o
	gcc -o qtst1 qtst1.o qincb.o qincbi.o incbet.o beta.o incbi.o -lmq -lmd

#	gcc -o qtst1 qtst1.o beta.o dbetai.o incbet.o incbi.o \
#qincbi.o incbil.o incbetl.o -lmq -lml -lmd
