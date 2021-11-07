#!/usr/bin/env bash

##################################################
# Vagrant Ubuntu 20.04 Provisioning Script 
# 	
# Description:
# 	Provisions the VM for basic C++ development
# 		
##################################################

CWD=$(realpath .)

APT_INSTALL_PACKAGES=(
	wget
	git
	build-essential
	gdb
	clang
	cmake
	openssl
	valgrind
	python3-pip
)

PIP_INSTALL_PACKAGES=(
	requests
)

# update list of available packages
echo "updating apt-get..."
sudo apt-get update -y

# upgrade installed packages
echo "upgrading installed packages..."
sudo apt-get upgrade -y

# install new packages
echo "installing packages: ${APT_INSTALL_PACKAGES[@]}"
sudo apt-get install -y "${APT_INSTALL_PACKAGES[@]}"
echo "finished installing apt packages!"

# install python deps
echo "installing python packages: ${PIP_INSTALL_PACKAGES[@]}"
pip3 install --no-input "${PIP_INSTALL_PACKAGES[@]}"
echo "finished installing python packages!"

# install unicorn
echo "installing unicorn..."

# specify unicorn source directory
UNICORN_DIR="/home/vagrant/unicorn-latest"

# download the unicorn source using python
echo "downloading unicorn source..."
python3 <<< "
import requests, json
r = requests.get('https://api.github.com/repos/unicorn-engine/unicorn/releases/latest')
v = json.loads(r.text)
r = requests.get(v['tarball_url'])
with open('unicorn-latest.tar.gz', 'wb') as f:
	f.write(r.content)
"

# extract the tarball
echo "extracting unicorn tarball..."
if [ -f "unicorn-latest.tar.gz" ]; then
	mkdir $UNICORN_DIR && \
		tar -zxvf "unicorn-latest.tar.gz" -C $UNICORN_DIR --strip-components=1 && \
		rm "unicorn-latest.tar.gz"
else
	echo "Error: unicorn-latest.tar.gz not found!"
	exit
fi

# compile unicorn and install
echo "compiling and installing unicorn..."
cd $UNICORN_DIR
UNICORN_ARCHS="arm" ./make.sh
sudo UNICORN_ARCHS="arm" ./make.sh install
cd -
echo "finished installing unicorn!"

# download systemc (leave customization for later)
echo "installing systemc..."

# specify systemc source directory
SYSTEMC_DIR="/home/vagrant/systemc"

echo "downloading systemc source..."
wget "https://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.tar.gz" -O "systemc.tar.gz"
echo "extracting systemc source..."
mkdir $SYSTEMC_DIR && \
	tar -zxvf "systemc.tar.gz" -C $SYSTEMC_DIR --strip-components=1 && \
	rm "systemc.tar.gz"

# compile and install systemc
echo "compiling and building systemc..."
cd $SYSTEMC_DIR
mkdir build && cd build
export CXX=clang++
sudo ../configure --with-unix-layout --prefix="/usr/"
sudo make
sudo make check
sudo make install
cd -
echo "finished installing systemc!"

# install arm-non-eabi ARM Embedded Toolchain
echo "downloading gcc-arm-none-eabi..."
cd /usr/local/lib/
sudo wget "https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2" -O "gcc-arm-none-eabi.tar.bz2"
# checksum
if [ $(md5sum "gcc-arm-none-eabi.tar.bz2") != "2383e4eb4ea23f248d33adc70dc3227e" ]; then
	echo "ERROR: gcc-arm-none-eabi checksum failed!"
	exit
fi
echo "extracting gcc-arm-none-eabi..."
tar -jxvf "gcc-arm-none-eabi.tar.bz2"
sudo rm "gcc-arm-none-eabi.tar.bz2"
sudo ln -s /usr/local/lib/gcc-arm-none-eabi/bin/* "/usr/local/bin/"
# echo "simlink libncurses and libtinfo for gcc-arm-none-eabi dependency"
sudo ln -s "/usr/lib/x86_64-linux-gnu/libncurses.so.6" "/usr/lib/x86_64-linux-gnu/libncurses.so.5"
sudo ln -s "/usr/lib/x86_64-linux-gnu/libtinfo.so.6" "/usr/lib/x86_64-linux-gnu/libtinfo.so.5"
cd -
echo "finished installing gcc-arm-none-eabi!"

echo -e "\n\nfinished!"
