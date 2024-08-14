SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)
cd $SHELL_FOLDER/execute
insmod wbs2812bDriver.ko
echo "Driver: Install ws2812b driver."
insmod buzzerdriver.ko
echo "Driver: Install buzzer driver."
insmod dht11Driver.ko
echo "Driver: Install dht11 driver."
insmod ledDriver.ko
echo "Driver: Install 4 Pcs Led driver."
insmod hchr04driver.ko
echo "Driver: Install HC-HR04 driver."