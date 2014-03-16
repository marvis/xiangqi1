all:
	g++ -o bin/splitchess splitchess.cpp `pkg-config --cflags --libs opencv`
