all:
	g++ -o xq_recog_cmd xq_recog_cmd.cpp `pkg-config --cflags --libs opencv`
	#g++ -o xq_recog xq_recog.cpp `pkg-config --cflags --libs opencv`
