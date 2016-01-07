#!/bin/bash

sudo rmmod my_module &> /dev/null
sudo make -C /usr/src/linux-headers-$(uname -r) M=$(pwd) modules

sudo insmod ./my_module.ko
major=$(cat /proc/devices | grep ogrodje | cut -f 1 -d " ")
sudo rm -R /dev/my_module &> /dev/null
sudo mknod -m 777 /dev/my_module c $major 0
