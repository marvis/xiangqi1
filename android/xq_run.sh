export LD_LIBRARY_PATH=/data/local/flappybird/opencv_libs:$LD_LIBRARY_PATH
# step1: check the start fenstr
#rm *.png
/system/bin/screencap screen.png
result=`./xq_recog_cmd screen.png`;
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
lasttime=`./current_time`
while [ 1 ]
do
	/system/bin/screencap screen.png
	screen=`./whichscreen ./screen.png`
	if [[ "$screen" == "startview" || "$screen" == "selectview" || "$screen" == "loginview" || "$screen" == "backview" || "$screen" == "confirmview" || "$screen" == "neterrorview" || "$screen" == "enterview" || "$screen" == "prepareview" || "$screen" == "changingtableview" || "$screen" == "gameoverview" ]]; then
		echo "$screen exit"
		#./msg.sh "$screen exit"
		break
	elif [ "$screen" == "unknowview" ]; then
		echo "$screen continue"
		#./msg.sh "$screen continue"
		continue
	fi

	result=`./xq_recog_cmd screen.png`
	fenstr=`echo $result | awk '{print $1}'`
	#echo $fenstr
	if [ "$fenstr" == "$prev_fenstr" ]; then
		echo "no moves"
		#./msg.sh "no moves"
		#sleep 0.2
		continue
	fi

	isvalid1=`./isfenvalid $fenstr`
	if [ "$isvalid1" == "false" ]; then 
		echo "fenstr is valid"
		#./msg.sh "fenstr is valid"
		#sleep 0.2
		failcount=$[failcount+1]
		if [ $failcount -ge 4 ]; then break; fi
		continue; 
	fi

	isvalid2=`./isfenvalid $prev_fenstr $fenstr`
	if [ "$isvalid2" == "false" ]; then
		echo "fenstr is not from prev_fenstr"
		#./msg.sh "fenstr is not from prev_fenstr"
		#sleep 0.2
		failcount=$[failcount+1]
		if [ $failcount -ge 4 ]; then break; fi
		continue;
	fi
	failcount=0

	clear
	echo "-------------------------------"
	./fenstr2matrix.sh $fenstr `./whichmoves $prev_fenstr $fenstr`
	echo "-------------------------------"
	curtime=`./current_time`; echo "time lapsed: `expr $curtime - $lasttime`"; lasttime=$curtime;

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
		#./msg.sh "game over"
		break
	fi

	clear
	echo "-------------------------------"
	./fenstr2matrix.sh $prev_fenstr `./whichmoves $fenstr $prev_fenstr`
	echo "-------------------------------"
	curtime=`./current_time`; echo "time lapsed: `expr $curtime - $lasttime`"; lasttime=$curtime;

	#sleep 0.2
done
