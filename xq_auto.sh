rm *.png
fid=1
start_time=`date "+%s"`
while [ 1 ]
do
	cur_time=`date "+%s"`
	if [ `expr $cur_time - $start_time` -ge 28000 ]; then break; fi
	#rm screen.png
	adb shell screencap /storage/sdcard0/screen.png && adb pull /storage/sdcard0/screen.png ./ >> /dev/null 2>&1
	isstart=`./isstartview ./screen.png`
	if [ $isstart == "true" ]; then
		echo "start view"
		cp ./screen.png ./screen${fid}_start.png; fid=$[fid+1]
		while [ 1 ]
		do
			isprepare=`./isprepareview ./screen.png`
			if [ $isprepare == "true" ]; then 
				echo "prepared"
				cp ./screen.png ./screen${fid}_prepare.png; fid=$[fid+1]
				./adb/touch.sh 546 720
				sleep 1
				echo "start play1"
				./xq_run.sh
				break
			else
				echo "not prepared"
				cp ./screen.png ./screen${fid}_notprepare.png; fid=$[fid+1]
				./adb/touch.sh 194 720
				sleep 1
			fi
			adb shell screencap /storage/sdcard0/screen.png && adb pull /storage/sdcard0/screen.png ./ >> /dev/null 2>&1
		done

		continue
	fi

	isselect=`./isselectview ./screen.png`
	if [ $isselect == "true" ]; then
		echo "select view"
		cp ./screen.png ./screen${fid}_select.png; fid=$[fid+1]
		./adb/touch.sh 357 513
		sleep 1
		continue
	fi

	isback=`./isbackview ./screen.png`
	if [ $isback == "true" ]; then
		echo "back view"
		cp ./screen.png ./screen${fid}_back.png; fid=$[fid+1]
		./adb/touch.sh 310 728
		sleep 1
		continue
	fi

	isplay=`./isplayview ./screen.png`
	if [ $isplay == "true" ]; then
		echo "start play2"
		cp ./screen.png ./screen${fid}_play.png; fid=$[fid+1]
		./xq_run.sh
		continue
	fi
	isenter=`./isenterview ./screen.png`
	if [ $isenter == "true" ]; then
		echo "enter view"
		cp ./screen.png ./screen${fid}_enter.png; fid=$[fid+1]
		sleep 1
		continue
	fi
	echo "unknown view"
	cp ./screen.png ./screen${fid}_unknow.png; fid=$[fid+1]
done
