#!/bin/bash

set -e

sudo apt update -qq
sudo apt install libjsoncpp-dev -y

#
#
#

sudo apt remove cmake -y || true
sudo snap install cmake --classic || sudo snap refresh cmake --classic

sudo apt install ninja-build -y

#
# Install Clang 17
#
if [[ ! -f /usr/bin/clang++-17 ]]; then
  wget https://apt.llvm.org/llvm.sh
  chmod u+x llvm.sh
  sudo ./llvm.sh 17
  clang-17 --version
fi
echo "CMAKE_C_COMPILER   = /usr/bin/clang-17"
echo "CMAKE_CXX_COMPILER = /usr/bin/clang++-17"

#sudo apt install -y libsdl2-dev

#
#
#

bash ./bin/install-lua53.sh
bash ./bin/install-ogre.sh
#bash ./bin/install-ogre-next.sh
bash ./bin/install-irrlicht1.8.sh
#bash ./bin/install-OIS.sh
