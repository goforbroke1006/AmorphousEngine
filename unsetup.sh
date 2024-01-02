#!/bin/bash

sudo rm -rf /usr/local/include/OGRE/
sudo rm -rf /usr/local/lib/OGRE/
sudo rm -f /usr/local/lib/libOgre*
sudo rm -f /usr/local/lib/pkgconfig/OGRE*
sudo rm -rf /usr/local/share/OGRE/

sudo rm -rf /usr/local/include/LuaCpp/
sudo rm -f /usr/local/lib/libluacpp*
sudo rm -rf /usr/local/lib/LuaCpp/

sudo apt remove lua5.3 liblua5.3-dev
