#!/bin/bash

sudo rm -f \
  /usr/local/lib/libOgre*         \
  /usr/local/bin/OgreXMLConverter \
  /usr/local/bin/VRMLConverter    \
  /usr/local/bin/OgreMeshUpgrader
sudo rm -rf \
  /usr/local/include/OGRE/    \
  /usr/local/lib/OGRE/        \
  /usr/local/share/OGRE-14.1/ \
  /usr/local/share/doc/OGRE/

# Remove LuaCPP
sudo rm -f /usr/local/lib/libluacpp*
sudo rm -rf /usr/local/include/LuaCpp/ /usr/local/lib/LuaCpp/

sudo apt remove lua5.3 liblua5.3-dev -y

sudo apt-get remove libirrlicht1.8 libirrlicht-dev libirrlicht-doc -y

# Deps for ColladaOgreImporter
#sudo apt remove -y \
#  libxml2-dev libpcre3-dev libboost-filesystem-dev

sudo apt remove -y libsdl2-dev

# clear OIS
sudo rm -rf /usr/local/include/ois/
sudo rm -f /usr/local/lib/libOIS.*
sudo rm -f /usr/local/lib/pkgconfig/OIS.pc

