#!/usr/bin/env bash

set -e

mkdir -p cmake-build-release
(
  cd cmake-build-release

  cmake -G Ninja \
    -DCMAKE_MAKE_PROGRAM=ninja \
    -DCMAKE_C_COMPILER=/usr/bin/clang-17 \
    -DCMAKE_CXX_COMPILER=/usr/bin/clang++-17 \
    -DCMAKE_BUILD_TYPE=Release \
    ..
  cmake --build .
)

#
#
#

#rm -rf ./release
mkdir -p ./release

cat <<EOT > ./release/launcher
#!/usr/bin/env bash

set -e

SCRIPT_DIR="\$( cd "\$( dirname "\${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="\${1:-\$PWD}"

PROJECT_DIR=\$(realpath "\$(pwd)/\${PROJECT_DIR}")

cd "\${SCRIPT_DIR}"

echo "Engine:  \${SCRIPT_DIR}"
echo "Project: \${PROJECT_DIR}"
LD_LIBRARY_PATH=. ./AmorphousEngine Ogre "\${SCRIPT_DIR}" "\${PROJECT_DIR}" "\${PROJECT_DIR}/Scenes/level-0.json"

EOT

chmod +x ./release/launcher

cp ./third_party/luacpp/build/libluacpp.so ./release/

cp ~/ogre/build/Release/lib/*.so ./release/
cp ~/ogre/build/Release/lib/*.so.* ./release/

cp /usr/lib/x86_64-linux-gnu/libIrrlicht.so.1.8.5 ./release/
cp /usr/lib/x86_64-linux-gnu/libIrrlicht.so.1.8 ./release/

rm -rf release/Component
cp -r ./Component/ ./release/

rm -rf release/Core
cp -r ./Core/ ./release/

sudo cp /usr/local/lib/libOIS.so.1.5.0 ./release/
sudo cp /usr/local/lib/libOIS.so.1.5.1 ./release/

cp ./plugins.cfg ./release/

cp ./cmake-build-release/AmorphousEngine ./release/

rm -rf ./release/projects/
cp -r ./projects/ ./release/

cat <<EOT > ./release/ogre.cfg
Render System=OpenGL 3+ Rendering Subsystem

[OpenGL 3+ Rendering Subsystem]
Colour Depth=
Display Frequency=N/A
FSAA=0
Full Screen=No
RTT Preferred Mode=FBO
VSync=No
Video Mode= 800 x  600
sRGB Gamma Conversion=Yes

EOT

OS_NAME='unknown'
OS_ARCH='unknown'

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  OS_NAME='linux'
  OS_ARCH=$(dpkg --print-architecture)
elif [[ "$OSTYPE" == "darwin"* ]]; then
  OS_NAME='darwin'
elif [[ "$OSTYPE" == "cygwin" ]]; then
  OS_NAME='windows'
elif [[ "$OSTYPE" == "msys" ]]; then
  OS_NAME='windows'
elif [[ "$OSTYPE" == "win32" ]]; then
  OS_NAME='windows'
  OS_ARCH='386'
elif [[ "$OSTYPE" == "freebsd"* ]]; then
  OS_NAME='freebsd'
else
  OS_NAME='unknown'
  OS_ARCH='unknown'
fi

sudo chmod -R 0777 ./release

rm -f "./release-${OS_NAME}-${OS_ARCH}.zip"
zip -r "./release-${OS_NAME}-${OS_ARCH}" ./release
