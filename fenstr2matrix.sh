fenstr=$1
i=0
while [ $i -lt ${#fenstr} ]
do
	c=${fenstr:i:1}
	if [ $c == r ]; then echo -n -e "[1;30m车[0m "
	elif [ $c == n ]; then echo -n -e "[1;30m马[0m "
	elif [ $c == b ]; then echo -n -e "[1;30m象[0m "
	elif [ $c == a ]; then echo -n -e "[1;30m士[0m "
	elif [ $c == k ]; then echo -n -e "[1;30m将[0m "
	elif [ $c == c ]; then echo -n -e "[1;30m炮[0m "
	elif [ $c == p ]; then echo -n -e "[1;30m卒[0m "
	elif [ $c == R ]; then echo -n -e "[1;31m车[0m "
	elif [ $c == N ]; then echo -n -e "[1;31m马[0m "
	elif [ $c == B ]; then echo -n -e "[1;31m相[0m "
	elif [ $c == A ]; then echo -n -e "[1;31m仕[0m "
	elif [ $c == K ]; then echo -n -e "[1;31m将[0m "
	elif [ $c == C ]; then echo -n -e "[1;31m炮[0m "
	elif [ $c == P ]; then echo -n -e "[1;31m兵[0m "
	elif [ $c == '/' ]; then echo ""
	else
		j=1
		while [ $j -le $c ]
		do
			echo -n -e "   "
			j=$[j+1]
		done
	fi
	i=$[i+1]
done
echo ""
