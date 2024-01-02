#!/bin/bash

set -e

sudo apt update -qq
sudo apt install libjsoncpp-dev -y

bash ./bin/install-lua53.sh
bash ./bin/install-ogre-next.sh
