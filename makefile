#template directory for directory in practice modules
#doesn't work except from within the directory
MODULE= reverse

COMP = g++
CCFLAGS= -std=g++17

SRCDIR= src
EXECDIR= exec
INCDIR= include
LIBDIR= lib

LIBFILE= $(LIBDIR)/lib$(MODULE).a

lib:
	g++ -c ./(SRCDIR)/(MODULE).lib.cc ./(SRCDIR)/(MODULE).lib.hh -o ./(SRCDIR)/(MODULE).lib.o

exec:
	g++ -std=c++11 ./(SRCDIR)/(MODULE).cli.cc ./(SRCDIR)/(MODULE).lib.o -o ./(EXECDIR)/(MODULE)

run:
	cd (EXECDIR)
	./(MODULE)

clean:
	rm *.o (MODULE)
