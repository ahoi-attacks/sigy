#!/bin/bash

#
# first parameter is pid
#

echo "using pid $1"
sudo rmmod sig_usr_inject.ko
sudo insmod ./sig_usr_inject.ko pid=$1

sudo dmesg
