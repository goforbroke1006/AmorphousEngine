#!/usr/bin/env bash

set -e

sudo apt update -qq
sudo apt install lua5.3 liblua5.3-dev -y
sudo apt install libjsoncpp-dev -y
lua -v

mkdir -p third_party
(
  cd third_party

  if [[ ! -d ./luacpp/ ]]; then
    git clone https://github.com/jordanvrtanoski/luacpp.git
  else
    cd luacpp
    git pull
    cd ..
  fi

  (
    cd luacpp
    mkdir -p build
    cd build

    cmake ../Source
    make
    sudo make install
  )
)
