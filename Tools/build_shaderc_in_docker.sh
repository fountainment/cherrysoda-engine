#!/bin/bash

cd ~
apt update
apt-get install --reinstall ca-certificates
update-ca-certificates --fresh
c_rehash
if [ ! -d "/opt/cmake-3.12.2" ]; then
  wget https://cmake.org/files/v3.12/cmake-3.12.2-Linux-x86_64.tar.gz
  tar zxvf cmake-3.12.2-Linux-x86_64.tar.gz
  mv cmake-3.12.2-Linux-x86_64 /opt/cmake-3.12.2
  ln -sf /opt/cmake-3.12.2/bin/* /usr/bin/
fi
apt -y install libsdl2-dev
if [ -d "~/cherrysoda-engine/build" ]; then
  rm -rf ~/cherrysoda-engine/build
fi
mkdir ~/cherrysoda-engine/build
cd ~/cherrysoda-engine/build
cmake .. -DCHERRYSODA_BUILD_SHADERC=1 -DCHERRYSODA_USE_SSE2=1
make shaderc -j8
