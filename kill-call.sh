
if pgrep -x "chrome" > /dev/null
then
xdotool search --onlyvisible --name 'Chrome' windowactivate --sync key --clearmodifiers ctrl+w
fi
echo stop
sleep 1
