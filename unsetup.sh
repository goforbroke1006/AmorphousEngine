#!/bin/bash

read -p "Do you want to remove local builds? (yes/no) " yn
case $yn in
yes)
  sudo rm -rf ./third_party/luacpp/build/
  sudo rm -rf ./third_party/ogre-next-deps/build/
  sudo rm -rf ./third_party/ogre-next/build/
  ;;
no)
  echo Leave local builds
  ;;
*)
  echo invalid response
  exit 1
  ;;
esac

sudo rm -rf /usr/local/include/OGRE/
sudo rm -rf /usr/local/lib/OGRE/
sudo rm -f /usr/local/lib/libOgre*
sudo rm -f /usr/local/lib/pkgconfig/OGRE*
sudo rm -rf /usr/local/share/OGRE/

sudo rm -rf /usr/local/include/LuaCpp/
sudo rm -f /usr/local/lib/libluacpp*
sudo rm -rf /usr/local/lib/LuaCpp/

sudo apt remove lua5.3 liblua5.3-dev

sudo apt-get remove libirrlicht1.8 libirrlicht-dev libirrlicht-doc

# Deps for ColladaOgreImporter
#sudo apt remove -y \
#  libxml2-dev libpcre3-dev libboost-filesystem-dev
