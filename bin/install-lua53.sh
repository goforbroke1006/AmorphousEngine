#!/usr/bin/env bash

CXX_STANDARD=14     # 11 | 14

set -e

sudo apt update -qq
sudo apt install lua5.3 liblua5.3-dev -y
sudo apt install libjsoncpp-dev -y
sudo apt-get install valgrind -y
lua -v

mkdir -p third_party
(
  cd third_party

  if [[ ! -d ./luacpp/ ]]; then
    git clone https://github.com/jordanvrtanoski/luacpp.git
  else
    cd luacpp
    git checkout main
    git pull
    cd ..
  fi

  (
    cd luacpp
    mkdir -p build
    cd build

    cmake -G Ninja \
      -DCMAKE_MAKE_PROGRAM=ninja \
      -DCMAKE_C_COMPILER=/usr/bin/clang-17 \
      -DCMAKE_CXX_COMPILER=/usr/bin/clang++-17 \
      -DCMAKE_CXX_STANDARD=${CXX_STANDARD} \
      ../Source
#    make
#    sudo make install
    cmake --build .
    sudo cmake --install . --config Debug
  )
)
