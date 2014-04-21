all:
	g++ -o current_time current_time.cpp `pkg-config --cflags --libs opencv`
	g++ -o whichscreen whichscreen.cpp `pkg-config --cflags --libs opencv`
	g++ -o whichlevel whichlevel.cpp `pkg-config --cflags --libs opencv`
	g++ -o isnonoccupyview isnonoccupyview.cpp `pkg-config --cflags --libs opencv`
	g++ -o isprofileview isprofileview.cpp `pkg-config --cflags --libs opencv`
	g++ -o isfenvalid isfenvalid.cpp
	g++ -o xq_recog_cmd xq_recog_cmd.cpp `pkg-config --cflags --libs opencv`
	g++ -o fenstr2matrix fenstr2matrix.cpp
	g++ -o whichmoves whichmoves.cpp
clean:
	rm current_time
	rm whichscreen
	rm whichlevel
	rm isnonoccupyview
	rm isprofileview
	rm isfenvalid
	rm xq_recog_cmd
	rm fenstr2matrix
	rm whichmoves
mobile:
	cd android/current_time/jni && make
	cd android/whichscreen/jni && make
	cd android/whichlevel/jni && make
	cd android/isnonoccupyview/jni && make
	cd android/isprofileview/jni && make
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
