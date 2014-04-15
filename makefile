all:
	g++ -o isplayview isplayview.cpp `pkg-config --cflags --libs opencv`
	g++ -o isbackview isbackview.cpp `pkg-config --cflags --libs opencv`
	g++ -o isselectview isselectview.cpp `pkg-config --cflags --libs opencv`
	g++ -o isprepareview isprepareview.cpp `pkg-config --cflags --libs opencv`
	g++ -o isstartview isstartview.cpp `pkg-config --cflags --libs opencv`
	#g++ -o isfenvalid isfenvalid.cpp
	#g++ -o xq_recog_cmd xq_recog_cmd.cpp `pkg-config --cflags --libs opencv`
	#g++ -o xq_recog xq_recog.cpp `pkg-config --cflags --libs opencv`
