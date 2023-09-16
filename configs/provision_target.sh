#!/usr/bin/env bash

echo ">>> Configuring telnet connections"
apt-get update -y
apt-get install -y telnetd gcc
echo systemctl status inetd
echo ">>> Done"
