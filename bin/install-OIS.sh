#!/usr/bin/env bash

set -e

mkdir -p third_party/
(
  cd third_party/
  [[ -d OIS ]] || git clone --depth 1 --single-branch --branch v1.5.1 https://github.com/wgois/OIS.git

  (
    cd OIS
    cmake -H. -B./build
    (
      cd ./build
      make
      sudo make install
    )
  )
)
