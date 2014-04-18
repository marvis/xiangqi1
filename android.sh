for i in isloginview isneterrorview ischangingtableview isnonoccupyview whichlevel isprofileview isenterview isplayview isbackview isselectview isprepareview isstartview isfenvalid xq_recog_cmd fenstr2matrix isconfirmview whichmoves
do
	mkdir -p android/$i/jni
	cp android/common/Application.mk android/$i/jni/
	cp android/common/path.h android/$i/jni/
	cat android/common/Android.mk | sed 's/whichbkg/'$i'/' > android/$i/jni/Android.mk
	cat android/common/makefile | sed 's/whichbkg/'$i'/' > android/$i/jni/makefile
	rm android/$i/jni/$i.cpp
	cp $i.cpp android/$i/jni/$i.cpp
done
