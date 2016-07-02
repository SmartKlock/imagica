#!/bin/bash
input=/tmp/serverinput
output=/tmp/serveroutput
error=/tmp/servererror
port=1234

trap "rm -f $input" EXIT
if [[ ! -p $input ]];then
	mkfifo $input;
fi
trap "rm -f $output" EXIT
if [[ ! -p $output ]];then
	mkfifo $output;
fi
trap "rm -f $error" EXIT
if [[ ! -p $error ]];then
	mkfifo $error;
fi

echo "starting on port $port";
nohup sudo echo test  <$input 1>$output 2>$error;
echo "started";
while true
do
	if read line <$output; then
		if [[ "$output" == "Closing Server" ]];then
			echo "restarting program on port $port";
			sudo ../final/server $port <$input 1>$output 2>$error & disown;
		fi
		if [[ "$output" == "Error on binding" ]];then
			echo "binding error on port $port, closing server";
			echo S>$input;
			port=$(expr "$port" + 1 ) ;
			echo "changing port number to $port";
		fi
	fi
done

rm $input 2>/log1.txt
rm $output 2>/log1.txt
rm $error 2>/log1.txt
