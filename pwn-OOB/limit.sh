#!/bin/sh
sysctl -w kernel.dmesg_restrict=1
mount -o remount,hidepid=2 /proc
chmod 1733 /tmp /var/tmp /dev/shm
chown root:root /home/bot
iptables -F
iptables -A INPUT -d 10.0.0.0/8 -j DROP
iptables -A OUTPUT -d 10.0.0.0/8 -j DROP
