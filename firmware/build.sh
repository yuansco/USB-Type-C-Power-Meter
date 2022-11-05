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
ELF=$(ls ./Build/*.elf)

# ld file
LD=$(ls ./*.ld)

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

    # get chip Flash and SRAM size
    flash_full=$(cat $LD |grep "FLASH (rx)" |cut -d '=' -f 3 |cut -d 'K' -f 1 |tr -d ' ')
    sram_full=$(cat $LD |grep "RAM (xrw)" |cut -d '=' -f 3 |cut -d 'K' -f 1 |tr -d ' ')
    flash_used=$(($flash*100 / ($flash_full*1024)))
    sram_used=$(($sram*100 / ($sram_full*1024)))

    echo
    printf "FLASH used = %5s bytes (%2s%%)\n" "$flash" "$flash_used"
    printf "SRAM  used = %5s bytes (%2s%%)\n" "$sram" "$sram_used"
else
    echo "$ELF not exists."
fi

exit 0
