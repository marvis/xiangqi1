# step1: check the start fenstr
me_first="rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR"
other_first="RNBAKABNR/9/1C5C1/P1P1P1P1P/9/9/p1p1p1p1p/1c5c1/9/rnbakabnr"
mycolor="w"
pre_stauts=""
while [ 1 ]
do
	#adb shell screencap -p | perl -pe 's/\x0D\x0A/\x0A/g' > screen.png
	adb shell screencap /storage/sdcard0/screen.png && adb pull /storage/sdcard0/screen.png ./
	fenstr=`xq_recog_cmd screen.png`
	echo $fenstr
	if [ "$fenstr" == "$me_first" ]; then
		mycolor="w"
		pre_fenstr=""
		break
	fi
	if [ "$fenstr" == "$other_first" ]; then
		mycolor="b"
		pre_fenstr="$fenstr"
		break
	fi
	sleep 1
done

echo "=== start move ==="

#step2: start play
pre_fenstr=""
while [ 1 ]
do
	#adb shell screencap -p | perl -pe 's/\x0D\x0A/\x0A/g' > screen.png
	adb shell screencap /storage/sdcard0/screen.png && adb pull /storage/sdcard0/screen.png ./
	fenstr=`xq_recog_cmd screen.png`
	echo $fenstr
	
	if [ "$fenstr" == "$pre_fenstr" ]; then
		continue
		sleep 1
	fi
	
	isvalid=`./isfenvalid $prev_fenstr $fenstr`
	if [ "$isvalid" == "false" ]; then 
		echo "invalid fenstr"
		continue; 
	fi

	./fenstr2matrix $fenstr
	nextmove=`node xqwizard_interface/xqcli.js -v -f "$fenstr $mycolor"`
	echo $nextmove

	x0=`echo "$nextmove" | awk '{print $1}'`
	y0=`echo "$nextmove" | awk '{print $2}'`
	x1=`echo "$nextmove" | awk '{print $3}'`
	y1=`echo "$nextmove" | awk '{print $4}'`
	status=`echo "nextmove" | awk '{print $5}'`
    prev_fenstr1=`echo "$nextmove" | awk '{print $6}'`
	./adb/touch.sh $x0 $y0
	./adb/touch.sh $x1 $y1
	#adb shell screencap -p | perl -pe 's/\x0D\x0A/\x0A/g' > screen.png
	sleep 0.1
	adb shell screencap /storage/sdcard0/screen.png && adb pull /storage/sdcard0/screen.png ./
	prev_fenstr2=`xq_recog_cmd screen.png`
	if [ "$prev_fenstr1" != "$prev_fenstr2" ]; then
		echo "invalid move"
	fi
	prev_fenstr=$prev_fenstr1
	sleep 0.1
done
