# step1: check the start fenstr
rm *.png
mycolor=$1
pre_stauts=""
fid=1
failcount=0
#echo "=== start move ==="

#step2: start play
pre_fenstr=""
while [ 1 ]
do
	adb shell screencap /storage/sdcard0/screen.png && adb pull /storage/sdcard0/screen.png ./ >> /dev/null 2>&1
	fenstr=`xq_recog_cmd screen.png $fid.png`; fid=$[fid+1]
	if [ "$fenstr" == "$prev_fenstr" ]; then
		#echo -n -e ".."
		sleep 0.2
		continue
	fi

	isvalid1=`./isfenvalid $fenstr`
	if [ "$isvalid1" == "false" ]; then 
		#echo "invalid fenstr"
		sleep 0.2
		failcount=$[failcount+1]
		if [ $failcount -ge 10 ]; then break; fi
		continue; 
	fi

	isvalid2=`./isfenvalid $prev_fenstr $fenstr`
	if [ "$isvalid2" == "false" ]; then
		#echo "invalid fenstr from prev_fenstr"
		sleep 0.2
		failcount=$[failcount+1]
		if [ $failcount -ge 10 ]; then break; fi
		continue;
	fi
	#echo ""
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
	./adb/touch.sh $x0 $y0
	./adb/touch.sh $x1 $y1

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
