# step1: check the start status
me_first="rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR"
other_first="RNBAKABNR/9/1C5C1/P1P1P1P1P/9/9/p1p1p1p1p/1c5c1/9/rnbakabnr"
mycolor="w"
pre_stauts=""
while [ 1 ]
do
	adb shell screencap -p | perl -pe 's/\x0D\x0A/\x0A/g' > screen.png
	status=`xq_recog_cmd screen.png`
	echo $status
	if [ "$status" -eq "$me_first" ]; then
		mycolor="w"
		pre_status=""
		break
	fi
	if [ "$status" -eq "$other_first" ]; then
		mycolor="b"
		pre_status="$status"
		break
	fi
	sleep 1
done

pre_status=""
while [ 1 ]
do
	adb shell screencap -p | perl -pe 's/\x0D\x0A/\x0A/g' > screen.png
	status=`xq_recog_cmd screen.png`
	echo $status
	if [ "$status" == "$pre_status" ]; then
		continue
		sleep 1
	fi
	nextmove=`./xqwizard_interface/xqwizard_cmd.js -f "$status $mycolor"`
	x0=`cat $nextmove | awk "{$1*59}"`
	y0=`cat $nextmove | awk "{$2}"`
	x1=`cat $nextmove | awk "{$3}"`
	y1=`cat $nextmove | awk "{$4}"`
	echo ./adb/touch.sh $x0 $y0
	echo ./adb/touch.sh $x1 $y1
	adb shell screencap -p | perl -pe 's/\x0D\x0A/\x0A/g' > screen.png
	pre_status=`xq_recog_cmd screen.png`
	sleep 1
done
