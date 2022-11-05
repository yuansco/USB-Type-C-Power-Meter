# USB-Type-C-Power-Meter

This project is a open source USB Type-C power monitor based on STM32 for USB Type-C Power Delivery related tests.

<img src="https://github.com/yuansco/USB-Type-C-Power-Meter/blob/main/image/demo_gif.gif" style="width:550px;"/>

### How to building a new firmware binary

Download GNU Arm cross compiler toolchain:
```
https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q3-update/+download/gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar.bz2
```
Unzip toolchain and add it to PATH:
```
tar -vxjf gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar.bz2
echo "export PATH=$PATH:~/gcc-arm-none-eabi-5_4-2016q3/bin" >> ~/.bashrc
source ~/.bashrc
arm-none-eabi-gcc -v
```
Run `build.sh` script to compile:
```
./build.sh
```

