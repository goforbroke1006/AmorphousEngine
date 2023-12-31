#!/bin/bash

sudo rm -rf /usr/local/include/LuaCpp/

sudo rm -f /usr/local/lib/libluacpp*
sudo rm -rf /usr/local/lib/LuaCpp/

sudo apt remove lua5.3 liblua5.3-dev
