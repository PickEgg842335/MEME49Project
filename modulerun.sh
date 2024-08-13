SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)
cd $SHELL_FOLDER/execute
insmod wbs2812bDriver.ko
echo "Driver: Install ws2812b driver."
insmod buzzerdriver.ko
echo "Driver: Install buzzer driver."
