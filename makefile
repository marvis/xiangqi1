all:
	g++ -o bin/positivechess positivechess.cpp `pkg-config --cflags --libs opencv`
	#g++ -o bin/detectchess detectchess.cpp `pkg-config --cflags --libs opencv`
	#g++ -o bin/diffimage diffimage.cpp `pkg-config --cflags --libs opencv`
	#g++ -o bin/medianimage medianimage.cpp `pkg-config --cflags --libs opencv`
	#g++ -o bin/splitchess splitchess.cpp `pkg-config --cflags --libs opencv`
	#g++ -o bin/viewchess viewchess.cpp `pkg-config --cflags --libs opencv`
