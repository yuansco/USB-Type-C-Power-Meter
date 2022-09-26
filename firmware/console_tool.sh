#!/bin/bash

have_CP210x=`lsusb |grep 10c4:ea60 |grep CP210x`      # CP2102

if [[ "$have_CP210x" != "" ]]
then
    echo "Found USB device: CP2102"
fi

have_PL2303=`lsusb |grep 067b:2303 |grep PL2303`      # PL2303

if [[ "$have_PL2303" != "" ]]
then
    echo "Found USB device: PL2303"
fi

have_UART_device="$have_CP210x$have_PL2303"

have_ttyusb=`ls /dev/ttyUSB0 -l`

have_ttyusb_user=`ls /dev/ttyUSB0 -l |grep $USER`


if [[ "$have_UART_device" != "" && "$have_ttyusb" != "" && "$have_ttyusb_user" == "" ]]
then
    #echo "Run: sudo usermod -a -G dialout $USER"
    #sudo usermod -a -G dialout $USER
    
    echo "Run: sudo chown $USER /dev/ttyUSB0"
    sudo chown $USER /dev/ttyUSB0
    
    echo "Run: ls /dev/ttyUSB0 -l"
    ls /dev/ttyUSB0 -l
    exit 0
fi

