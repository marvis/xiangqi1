#rm *.png
fid=1
start_time=`date "+%s"`
while [ 1 ]
do
	cur_time=`date "+%s"`
	if [ `expr $cur_time - $start_time` -ge 28000 ]; then break; fi
	#rm screen.png
	adb shell screencap /storage/sdcard0/screen.png && adb pull /storage/sdcard0/screen.png ./ >> /dev/null 2>&1
	isstart=`./isstartview ./screen.png`
	if [ "$isstart" == "true" ]; then
		echo "start view"
		cp ./screen.png ./screen${fid}_start.png; fid=$[fid+1]
		
		isnonoccupy=`./isnonoccupyview ./screen.png`
		if [ "$isnonoccupy" == "true" ]; then
			echo "nonoccupy"
			cp ./screen.png ./screen${fid}_nonoccupy.png; fid=$[fid+1]
			./touch.sh 194 720
			sleep 1
			continue
		fi

		# touch the profile image
		./touch.sh 363 108
		sleep 1

		adb shell screencap /storage/sdcard0/screen.png && adb pull /storage/sdcard0/screen.png ./ >> /dev/null 2>&1
		isprofile=`./isprofileview ./screen.png`
		if [ "$isprofile" == "true" ]; then
			echo "profile view"
			cp ./screen.png ./screen${fid}_profile.png; fid=$[fid+1]
			./touch.sh 531 126
			sleep 1
		else
			echo "not profile view"
			continue
		fi
		level=`./whichlevel ./screen.png`
		echo "level = $level"
		cp ./screen.png ./screen${fid}_level$level.png; fid=$[fid+1]
		if [ "$level" -le 4 ]; then
			# touch start
			./touch.sh 546 720
			sleep 1
		else
			# change table
			./touch.sh 194 720
			sleep 1
		fi
		continue
	fi

	isselect=`./isselectview ./screen.png`
	if [ "$isselect" == "true" ]; then
		echo "select view"
		cp ./screen.png ./screen${fid}_select.png; fid=$[fid+1]
		./touch.sh 357 513
		sleep 1
		continue
	fi

	islogin=`./isloginview ./screen.png`
	if [ "$islogin" == "true" ]; then
		echo "login view"
		cp ./screen.png ./screen${fid}_login.png; fid=$[fid+1]
		./touch.sh 375 879
		sleep 1
		continue
	fi

	isback=`./isbackview ./screen.png`
	if [ "$isback" == "true" ]; then
		echo "back view"
		cp ./screen.png ./screen${fid}_back.png; fid=$[fid+1]
		./touch.sh 310 728
		sleep 1
		continue
	fi

	isconfirm=`./isconfirmview ./screen.png`
	if [ "$isconfirm" == "true" ]; then
		echo "confirm view"
		cp ./screen.png ./screen${fid}_confirm.png; fid=$[fid+1]
		./touch.sh 355 776
		sleep 1
		continue
	fi
	isneterror=`./isneterrorview ./screen.png`
	if [ "$isneterror" == "true" ]; then
		echo "neterror view"
		cp ./screen.png ./screen${fid}_neterror.png; fid=$[fid+1]
		./touch.sh 352 778
		sleep 1
		continue
	fi

	isenter=`./isenterview ./screen.png`
	if [ "$isenter" == "true" ]; then
		echo "enter view"
		cp ./screen.png ./screen${fid}_enter.png; fid=$[fid+1]
		sleep 1
		continue
	fi

	isprepare=`./isprepareview ./screen.png`
	if [ "$isprepare" == "true" ]; then
		echo "prepare view"
		cp ./screen.png ./screen${fid}_prepare.png; fid=$[fid+1]
		sleep 1
		continue
	fi

	ischangingtable=`./ischangingtableview ./screen.png`
	if [ "$ischangingtable" == "true" ]; then
		echo "changing table view"
		cp ./screen.png ./screen${fid}_changingtable.png; fid=$[fid+1]
		sleep 1
		continue
	fi
	
	# isplay should be the last checked
	isplay=`./isplayview ./screen.png`
	if [ "$isplay" == "true" ]; then
		echo "start play2"
		cp ./screen.png ./screen${fid}_play.png; fid=$[fid+1]
		./xq_run.sh
		continue
	fi
	echo "unknown view"
	cp ./screen.png ./screen${fid}_unknow.png; fid=$[fid+1]
done
