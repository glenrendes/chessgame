build:
	g++ -c chess.lib.cc -o chess.lib.o
	g++ -std=c++11 chess.cli.cc chess.lib.o -o chess

test:
	g++ -c chess.lib.cc -o chess.lib.o
	g++ -std=c++11 chess.cli.cc chess.lib.o -o chess
	./chess

clean:
	rm *.o chess
