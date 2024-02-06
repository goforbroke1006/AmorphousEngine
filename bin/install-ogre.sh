#!/usr/bin/env bash

OGRE_BRANCH=v14.1.2
CXX_STANDARD=14

set -e

sudo apt-get install libgles2-mesa-dev
sudo apt-get install libsdl2-dev libxt-dev libxaw7-dev doxygen

cd ~
if [[ -d ./ogre ]]; then
  cd ogre
  git checkout "${OGRE_BRANCH}"
  cd ..
else
  git clone --depth 1 --single-branch -b "${OGRE_BRANCH}" https://github.com/OGRECave/ogre.git
fi

cd ogre
mkdir -p build
cd build

(
  mkdir -p Debug
  cd Debug

  cmake -G Ninja \
    -DCMAKE_MAKE_PROGRAM=ninja \
    -DCMAKE_C_COMPILER=/usr/bin/clang-17 \
    -DCMAKE_CXX_COMPILER=/usr/bin/clang++-17 \
    -DCMAKE_CXX_STANDARD=${CXX_STANDARD} \
    ../../

  cmake --build . --config Debug
  sudo cmake --build . --config Debug --target install
)

(
  mkdir -p Release
  cd Release

  cmake -G Ninja \
    -DCMAKE_MAKE_PROGRAM=ninja \
    -DCMAKE_C_COMPILER=/usr/bin/clang-17 \
    -DCMAKE_CXX_COMPILER=/usr/bin/clang++-17 \
    -DCMAKE_CXX_STANDARD=${CXX_STANDARD} \
    ../../

  cmake --build . --config Release
  sudo cmake --build . --config Release --target install
)
