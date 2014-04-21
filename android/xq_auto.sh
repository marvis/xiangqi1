export LD_LIBRARY_PATH=/data/local/flappybird/opencv_libs:$LD_LIBRARY_PATH
rm *.png
fid=1
start_time=`date "+%s"`
while [ 1 ]
do
	cur_time=`date "+%s"`
	if [ `expr $cur_time - $start_time` -ge 28000 ]; then break; fi
	#rm screen.png
	/system/bin/screencap screen.png
	view=`./whichscreen screen.png`
	if [ "startview" == "$view" ]; then
		echo "start view"
		./msg.sh "start view"
		#cp ./screen.png ./screen${fid}_start.png; fid=$[fid+1]
		
		isnonoccupy=`./isnonoccupyview ./screen.png`
		if [ "$isnonoccupy" == "true" ]; then
			echo "nonoccupy"
			./msg.sh "nonoccupy"
			#cp ./screen.png ./screen${fid}_nonoccupy.png; fid=$[fid+1]
			# change table
			./touch.sh 194 720
			sleep 1
			continue
		fi

		# touch the profile image
		./touch.sh 363 108
		sleep 1

		/system/bin/screencap screen.png
		isprofile=`./isprofileview ./screen.png`
		if [ "$isprofile" == "true" ]; then
			echo "profile view"
			./msg.sh "profile view"
			#cp ./screen.png ./screen${fid}_profile.png; fid=$[fid+1]
			# move outside
			./touch.sh 531 126
			sleep 1
		else
			echo "not profile view"
			./msg.sh "not profile view"
			continue
		fi
		level=`./whichlevel ./screen.png`
		echo "level = $level"
		./msg.sh "level = $level"
		#cp ./screen.png ./screen${fid}_level$level.png; fid=$[fid+1]
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

	if [ "selectview" == "$view" ]; then
		echo "select view"
		./msg.sh "select view"
		#cp ./screen.png ./screen${fid}_select.png; fid=$[fid+1]
		./touch.sh 357 513
		sleep 1
		continue
	fi

	if [ "loginview" == "$view" ]; then
		echo "login view"
		./msg.sh "login view"
		#cp ./screen.png ./screen${fid}_login.png; fid=$[fid+1]
		./touch.sh 375 879
		sleep 1
		continue
	fi

	if [ "backview" == "$view" ]; then
		echo "back view"
		./msg.sh "back view"
		#cp ./screen.png ./screen${fid}_back.png; fid=$[fid+1]
		./touch.sh 310 728
		sleep 1
		continue
	fi

	if [ "confirmview" == "$view" ]; then
		echo "confirm view"
		./msg.sh "confirm view"
		#cp ./screen.png ./screen${fid}_confirm.png; fid=$[fid+1]
		./touch.sh 355 776
		sleep 1
		continue
	fi
	if [ "neterrorview" == "$view" ]; then
		echo "neterror view"
		./msg.sh "neterror view"
		#cp ./screen.png ./screen${fid}_neterror.png; fid=$[fid+1]
		./touch.sh 352 778
		sleep 1
		continue
	fi

	if [[ "enterview1" == "$view" || "enterview2" == "$view" ]]; then
		echo "enter view"
		./msg.sh "enter view"
		#cp ./screen.png ./screen${fid}_enter.png; fid=$[fid+1]
		sleep 1
		continue
	fi
	
	if [ "gameoverview" == "$view" ]; then
		echo "gameover view"
		./msg.sh "gameover view"
		#cp ./screen.png ./screen${fid}_gameover.png; fid=$[fid+1]
		sleep 1
		continue
	fi	

	if [ "prepareview" == "$view" ]; then
		echo "prepare view"
		./msg.sh "prepare view"
		#cp ./screen.png ./screen${fid}_prepare.png; fid=$[fid+1]
		sleep 1
		continue
	fi

	if [ "changingtableview" == "$view" ]; then
		echo "changing table view"
		./msg.sh "changing table view"
		#cp ./screen.png ./screen${fid}_changingtable.png; fid=$[fid+1]
		sleep 1
		continue
	fi
	
	# isplay should be the last checked
	if [ "playview" == "$view" ]; then
		echo "start play"
		./msg.sh "start play"
		#cp ./screen.png ./screen${fid}_play.png; fid=$[fid+1]
		./xq_run.sh
		continue
	fi
	echo "unknown view"
	./msg.sh "unknown view"
	#cp ./screen.png ./screen${fid}_unknow.png; fid=$[fid+1]
done
