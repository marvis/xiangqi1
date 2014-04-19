#am start -a android.intent.action.MAIN -e "$1" mytoastmessage -n com.rja.utility/.ShowToast
./am.sh start -a android.intent.action.MAIN -e message "$1" -n com.rja.utility/.ShowToast >> /dev/null 2>&1
