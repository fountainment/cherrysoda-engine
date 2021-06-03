#!/bin/bash

cd ~
wget https://cmake.org/files/v3.12/cmake-3.12.2-Linux-x86_64.tar.gz
tar zxvf cmake-3.12.2-Linux-x86_64.tar.gz
mv cmake-3.12.2-Linux-x86_64 /opt/cmake-3.12.2
ln -sf /opt/cmake-3.12.2/bin/* /usr/bin/
apt update
apt -y install libsd2-dev
mkdir ~/cherrysoda-engine/build
cd ~/cherrysoda-engine/build
cmake .. -DCHERRYSODA_BUILD_SHADERC=1 -DCHERRYSODA_USE_SSE2=1
make shaderc -j8
