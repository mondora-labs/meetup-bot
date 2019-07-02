
if !(pgrep -x "chrome" > /dev/null)
then
    ./start-chrome.sh &
	sleep 2
fi
xdotool search --onlyvisible --name 'Chrome' windowactivate --sync key --clearmodifiers ctrl+t
xdotool type 'meet.google.com'
xdotool key Return
sleep 3
xdotool keydown Shift
xdotool key Tab keyup Shift
xdotool key Return
sleep 6
xdotool key Ctrl+l
xdotool key ctrl+c
sleep 1
xdotool key Return
sleep 5
xdotool search --onlyvisible --name 'Chrome' windowactivate --sync key --clearmodifiers Return
sleep 1
a=`xsel -ob`
sleep 1
echo $a
sleep 1

