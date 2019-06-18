CCFLAGS=-std=c++11
LINKFLAGS=


chess.lib.o: chess.lib.cc chess.lib.hh
	g++ $(CCFLAGS) -c chess.lib.cc 

chess.cli.o: chess.cli.cc chess.lib.hh
	g++ $(CCFLAGS) -c chess.cli.cc

chess: chess.cli.o chess.lib.o
	g++ $(CCFLAGS) chess.cli.o chess.lib.o -o chess

clean:
	rm *.o chess
