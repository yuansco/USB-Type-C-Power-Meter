#!/bin/bash

############################
# clear old file
############################
make clean

############################
# Automatic build date
############################
BUILD_DATE=$(date +%Y.%m.%d)
LINE_NUMBER=$(cat ./Core/Inc/board.h |grep '#define BUILD_DATE' -n |cut -d ':' -f 1)
DATE_LINE="#define BUILD_DATE \"$BUILD_DATE\""
sed -i "$LINE_NUMBER s/ *.*/$DATE_LINE/" ./Core/Inc/board.h

############################
# build project
############################
make

############################
# show FLASH and SRAM size
############################

# elf file
ELF=$(ls ./build/*.elf)

# check elf file exists
if [ -e "$ELF" ]
then
    # calculate Flash and SRAM usage
    size_info=$(arm-none-eabi-size $ELF | awk NR\>1)
    text=$(echo "$size_info" | awk '{print $1}')
    data=$(echo "$size_info" | awk '{print $2}')
    bss=$(echo "$size_info" | awk '{print $3}')
    flash=$(($text + $data))
    sram=$(($bss + $data))
    echo
    echo "FLASH used = $flash bytes"
    echo "SRAM  used =  $sram bytes"
else
    echo "$ELF not exists."
fi

