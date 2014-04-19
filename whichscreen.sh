if [ "`./isstartview $1`" == "true" ]; then
	echo "startview"
elif [ "`./isselectview $1`" == "true" ]; then
	echo "selectview"
elif [ "`./isloginview $1`" == "true" ]; then
	echo "loginview"
elif [ "`./isbackview $1`" == "true" ]; then
	echo "backview"
elif [ "`./isconfirmview $1`" == "true" ]; then
	echo "confirmview"
elif [ "`./isneterrorview $1`" == "true" ]; then
	echo "neterrorview"
elif [ "`./isenterview $1`" == "true" ]; then
	echo "enterview"
elif [ "`./isprepareview $1`" == "true" ]; then
	echo "prepareview"
elif [ "`./ischangingtableview $1`" == "true" ]; then
	echo "changingtableview"
elif [ "`./isgameoverview $1`" == "true" ]; then
	echo "gameoverview"
elif [ "`./isplayview $1`" == "true" ]; then
	echo "playview"
else
	echo "unknowview"
fi
