SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)
OUT_PID=`pgrep -f meme4916.out`
forever stop $SHELL_FOLDER/execute/ErthItityServer.js
echo "Web Server: Stop working."
kill ${OUT_PID}
echo "System: Stop working."
rmmod wbs2812bDriver
echo "uninstall ws2812b driver."