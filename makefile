all:
	g++ -o current_time current_time.cpp `pkg-config --cflags --libs opencv`
	g++ -o whichscreen whichscreen.cpp `pkg-config --cflags --libs opencv`
	g++ -o isgameoverview isgameoverview.cpp `pkg-config --cflags --libs opencv`
	g++ -o isloginview isloginview.cpp `pkg-config --cflags --libs opencv`
	g++ -o isneterrorview isneterrorview.cpp `pkg-config --cflags --libs opencv`
	g++ -o isconfirmview isconfirmview.cpp `pkg-config --cflags --libs opencv`
	g++ -o ischangingtableview ischangingtableview.cpp `pkg-config --cflags --libs opencv`
	g++ -o isnonoccupyview isnonoccupyview.cpp `pkg-config --cflags --libs opencv`
	g++ -o whichlevel whichlevel.cpp `pkg-config --cflags --libs opencv`
	g++ -o isprofileview isprofileview.cpp `pkg-config --cflags --libs opencv`
	g++ -o isenterview isenterview.cpp `pkg-config --cflags --libs opencv`
	g++ -o isplayview isplayview.cpp `pkg-config --cflags --libs opencv`
	g++ -o isbackview isbackview.cpp `pkg-config --cflags --libs opencv`
	g++ -o isselectview isselectview.cpp `pkg-config --cflags --libs opencv`
	g++ -o isprepareview isprepareview.cpp `pkg-config --cflags --libs opencv`
	g++ -o isstartview isstartview.cpp `pkg-config --cflags --libs opencv`
	g++ -o isfenvalid isfenvalid.cpp
	g++ -o xq_recog_cmd xq_recog_cmd.cpp `pkg-config --cflags --libs opencv`
	g++ -o xq_recog xq_recog.cpp `pkg-config --cflags --libs opencv`
	g++ -o fenstr2matrix fenstr2matrix.cpp
	g++ -o whichmoves whichmoves.cpp
clean:
	rm current_time
	rm whichscreen
	rm isgameoverview
	rm isloginview
	rm isneterrorview
	rm isconfirmview
	rm ischangingtableview
	rm isnonoccupyview
	rm whichlevel
	rm isprofileview
	rm isenterview
	rm isplayview
	rm isbackview
	rm isselectview
	rm isprepareview
	rm isstartview
	rm isfenvalid
	rm xq_recog_cmd
	rm xq_recog
	rm fenstr2matrix
	rm whichmoves
mobile:
	cd android/current_time/jni && make
	cd android/whichscreen/jni && make
	cd android/isgameoverview/jni && make
	cd android/isloginview/jni && make
	cd android/isneterrorview/jni && make
	cd android/isconfirmview/jni && make
	cd android/ischangingtableview/jni && make
	cd android/isnonoccupyview/jni && make
	cd android/whichlevel/jni && make
	cd android/isprofileview/jni && make
	cd android/isenterview/jni && make
	cd android/isplayview/jni && make
	cd android/isbackview/jni && make
	cd android/isselectview/jni && make
	cd android/isprepareview/jni && make
	cd android/isstartview/jni && make
	cd android/isfenvalid/jni && make
	cd android/xq_recog_cmd/jni && make
	cd android/fenstr2matrix/jni && make
	cd android/whichmoves/jni && make
pushscript:
	adb push android/xq_auto.sh /data/local/xiangqi/
	adb push android/xq_run.sh /data/local/xiangqi/
	adb push android/am.sh /data/local/xiangqi/
	adb push android/msg.sh /data/local/xiangqi/
	adb push android/touch.sh /data/local/xiangqi/
pullscript:
	adb pull /data/local/xiangqi/xq_auto.sh android/
	adb pull /data/local/xiangqi/xq_run.sh android/
	adb pull /data/local/xiangqi/am.sh android/
	adb pull /data/local/xiangqi/msg.sh android/
	adb pull /data/local/xiangqi/touch.sh android/
