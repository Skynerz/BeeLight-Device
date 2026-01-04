#!/bin/bash

# Upload application only

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <firmware.bin>"
    exit 1
fi

which esptool.py > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo "esptool not found! Please install esptool to proceed."
    exit 1
fi

#esptool.py --chip auto --port /dev/ttyACM0 --baud 921600 --after hard_reset write_flash 0 $1 TODO
esptool.py --chip auto --port /dev/ttyACM0 --baud 921600 --after hard_reset write_flash 0x10000 $1
