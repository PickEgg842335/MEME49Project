SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)
insmod $SHELL_FOLDER/execute/wbs2812bDriver.ko
echo "Install ws2812b driver."