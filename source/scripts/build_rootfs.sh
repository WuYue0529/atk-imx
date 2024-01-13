#!/bin/sh

cd ../buildroot/buildroot-2019.02.6
dir=$(pwd)
echo "Enter buildroot src dir : ${dir}"

sudo make
# copy for nfs
cp output/images/rootfs.tar ../../../rootfs/ -f
cd ../../../rootfs/
tar -vxf rootfs.tar

# setenv bootargs 'console=tty1 console=ttymxc0,115200 root=/dev/nfs nfsroot=192.168.31.247:/home/zk/imx/rootfs rw ip=192.168.31.251:192.168.31.247:192.168.1.1:255.255.255.0::eth0:off'
# bootargs=console=ttymxc0,115200 root=/dev/nfs nfsroot=192.168.31.177:/home/charles/linux/rootfs/imx_6ull_alientek_alpha,proto=tcp rw ip=192.168.31.251:192.168.31.177:192.168.31.1:255.255.255.0::eth0:off
# setenv ipaddr 192.168.31.50;setenv ethaddr b8:ae:1d:01:00:00;setenv gatewayip 192.168.31.1;setenv netmask 255.255.255.0;setenv serverip 192.168.31.177;setenv bootargs 'console=ttymxc0,115200 root=/dev/nfs nfsroot=192.168.31.177:/home/charles/linux/rootfs/imx_6ull_alientek_alpha,