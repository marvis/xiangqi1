# step1: check the start fenstr
rm play*.png
fid=1
adb shell screencap /storage/sdcard0/screen.png && adb pull /storage/sdcard0/screen.png ./ >> /dev/null 2>&1
result=`xq_recog_cmd screen.png play$fid.png`; fid=$[fid+1];
fenstr=`echo $result | awk '{print $1}'`
mycolor=`echo $result | awk '{print $2}'`
whonext=`echo $result | awk '{print $3}'`
if [ $mycolor == $whonext ]; then
	pre_fenstr=""
else
	pre_fenstr=$fenstr
fi

failcount=0
#echo "=== start move ==="

#step2: start play
while [ 1 ]
do
	adb shell screencap /storage/sdcard0/screen.png && adb pull /storage/sdcard0/screen.png ./ >> /dev/null 2>&1

	screen=`./whichscreen ./screen.png`
	if [[ "$screen" == "startview" || "$screen" == "selectview" || "$screen" == "loginview" || "$screen" == "backview" || "$screen" == "confirmview" || "$screen" == "neterrorview" || "$screen" == "enterview" || "$screen" == "prepareview" || "$screen" == "changingtableview" || "$screen" == "gameoverview" ]]; then
		echo "$screen exit"
		break
	elif [ "$screen" == "unknowview" ]; then
		echo "$screen continue"
		continue
	fi

	result=`xq_recog_cmd screen.png play$fid.png`; fid=$[fid+1];
	fenstr=`echo $result | awk '{print $1}'`
	if [ "$fenstr" == "$prev_fenstr" ]; then
		#echo "对方没有动作"
		sleep 0.2
		continue
	fi

	isvalid1=`./isfenvalid $fenstr`
	if [ "$isvalid1" == "false" ]; then 
		echo "不合法的棋谱"
		sleep 0.2
		failcount=$[failcount+1]
		if [ $failcount -ge 4 ]; then break; fi
		continue; 
	fi

	isvalid2=`./isfenvalid $prev_fenstr $fenstr`
	if [ "$isvalid2" == "false" ]; then
		echo "不合法的走法"
		sleep 0.2
		failcount=$[failcount+1]
		if [ $failcount -ge 4 ]; then break; fi
		continue;
	fi
	failcount=0

	clear
	echo "-------------------------------"
	./fenstr2matrix.sh $fenstr `./whichmoves $prev_fenstr $fenstr`
	echo "-------------------------------"

	nextmove=`node xqwizard_interface/xqcli.js -v -f "$fenstr $mycolor"`
	#echo $nextmove

	x0=`echo "$nextmove" | awk '{print $1}'`
	y0=`echo "$nextmove" | awk '{print $2}'`
	x1=`echo "$nextmove" | awk '{print $3}'`
	y1=`echo "$nextmove" | awk '{print $4}'`
	status=`echo "$nextmove" | awk '{print $5}'`
    prev_fenstr=`echo "$nextmove" | awk '{print $6}'`
	#echo "start move"
	./touch.sh $x0 $y0
	./touch.sh $x1 $y1

	if [ "$status" == "1" ]; then
		echo "game over"
		break
	fi

	clear
	echo "-------------------------------"
	./fenstr2matrix.sh $prev_fenstr `./whichmoves $fenstr $prev_fenstr`
	echo "-------------------------------"

	sleep 0.2
done
