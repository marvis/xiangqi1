# step1: check the start fenstr
mycolor=$1
pre_stauts=""
fid=1
echo "=== start move ==="

#step2: start play
pre_fenstr=""
while [ 1 ]
do
	#adb shell screencap -p | perl -pe 's/\x0D\x0A/\x0A/g' > screen.png
	adb shell screencap /storage/sdcard0/screen.png && adb pull /storage/sdcard0/screen.png ./
	fenstr=`xq_recog_cmd screen.png $fid.png`; fid=$[fid+1]
	echo $fenstr
	if [ "$fenstr" == "$pre_fenstr" ]; then
		continue
		sleep 1
	fi

	isvalid=`./isfenvalid "$prev_fenstr" "$fenstr"`
	if [ "$isvalid" == "false" ]; then 
		echo "invalid fenstr"
		continue; 
	fi

	./fenstr2matrix $fenstr
	nextmove=`node xqwizard_interface/xqcli.js -v -f "$fenstr $mycolor"`
	#echo $nextmove

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
	prev_fenstr2=`xq_recog_cmd screen.png $fid.png`; fid=$[fid+1]
	echo "prev_fenstr1: $prev_fenstr1"
	echo "prev_fenstr2: $prev_fenstr2"
	if [ "$prev_fenstr1" != "$prev_fenstr2" ]; then
		echo "invalid move"
	fi
	prev_fenstr=$prev_fenstr1
	sleep 0.1
done
