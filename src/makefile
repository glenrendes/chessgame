CCFLAGS=-std=c++11
LINKFLAGS=

all: chess

accounting.lib.o: accounting.lib.cc accounting.lib.hh
	g++ $(CCFLAGS) -c accounting.lib.cc

chess.lib.o: chess.lib.cc chess.lib.hh Piece.cc Player.cc Move.cc Board.cc Game.cc
	g++ $(CCFLAGS) -c chess.lib.cc 

chess.cli.o: chess.cli.cc chess.lib.hh accounting.lib.cc accounting.lib.hh
	g++ $(CCFLAGS) -c chess.cli.cc

chess: chess.cli.o chess.lib.o accounting.lib.o
	g++ $(CCFLAGS) chess.cli.o chess.lib.o accounting.lib.o -o chess

clean:
	rm *.o chess
