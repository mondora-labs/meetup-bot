echo follow the instructions on screen to install all the llibraries and tools used by meetup-bot
sudo apt-get update
sleep 1
sudo apt-get upgrade
sleep 1
sudo apt  install cmake
sleep 1
sudo apt-get install git
sleep 1
sudo apt-get install xdotool
sleep 1
sudo apt-get install build-essential
sleep 1
sudo apt-get install libcurl4-openssl-dev
sleep 1
sudo apt-get update
sleep 1
git clone https://github.com/DaveGamble/cJSON
sleep 1
cd cJSON
mkdir build
cd build
cmake .. -DENABLE_CJSON_UTILS=On -DENABLE_CJSON_TEST=Off -DCMAKE_INSTALL_PREFIX=/usr
sleep 1
make
sleep 1
sudo make DESTDIR=$pkgdir install