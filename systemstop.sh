SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)
OUT_PID=`pgrep -f meme4916.out`
forever stop $SHELL_FOLDER/execute/ErthItityServer.js
echo "Web Server: Stop working."
kill -INT ${OUT_PID}
echo "System: Stop working."
rmmod wbs2812bDriver
echo "uninstall ws2812b driver."
rmmod buzzerdriver
echo "uninstall buzzer driver."
rmmod dht11Driver
echo "uninstall dht11 driver."
rmmod ledDriver
echo "uninstall 4 Pcs Led driver."
rmmod hchr04driver
echo "uninstall HC-HR04 driver."