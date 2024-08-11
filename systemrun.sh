SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)
cd $SHELL_FOLDER/execute
insmod wbs2812bDriver.ko
echo "Driver: Install ws2812b driver."
./meme4916.out &
echo "System: Start working."
forever start ErthItityServer.js
echo "Web Server: Start working."