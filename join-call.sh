URL1=$1
# echo $URL1
pgrep
if !(pgrep -x "chrome" > /dev/null)
then
	./start-chrome.sh
	sleep 2
fi
xdotool search --onlyvisible --name 'Chrome' windowactivate --sync key --clearmodifiers ctrl+t
xdotool type $URL1
sleep 1
xdotool key Return
sleep 5
xdotool key Return

