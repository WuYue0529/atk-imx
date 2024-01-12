#!/bin/sh

cd ../linux/linux-imx-rel_imx_4.1.15_2.1.0_ga
dir=$(pwd)
echo "Enter linux src dir : ${dir}"

# make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- clean
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- imx_alientek_emmc_defconfig
# make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j4

# copy for tftp download
cp arch/arm/boot/zImage ../../../image/ -f
cp arch/arm/boot/dts/imx6ull-alientek-emmc.dtb ../../../image/ -f
ls -al ../../../image/


# bootargs=console=ttymxc0,115200 root=/dev/nfs nfsroot=192.168.31.177:/home/charles/linux/rootfs/imx_6ull_alientek_alpha,proto=tcp rw ip=192.168.31.251:192.168.31.177:192.168.31.1:255.255.255.0::eth0:off
# setenv ipaddr 192.168.31.50;setenv ethaddr b8:ae:1d:01:00:00;setenv gatewayip 192.168.31.1;setenv netmask 255.255.255.0;setenv serverip 192.168.31.177;setenv bootargs 'console=ttymxc0,115200 root=/dev/nfs nfsroot=192.168.31.177:/home/charles/linux/rootfs/imx_6ull_alientek_alpha,