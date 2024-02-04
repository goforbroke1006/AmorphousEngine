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

rm -rf ./release
mkdir -p ./release

cat <<EOT > ./release/launcher
#!/usr/bin/env bash

SCRIPT_DIR="\$( cd "\$( dirname "\${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

cd "\${SCRIPT_DIR}"

LD_LIBRARY_PATH=. ./AmorphousEngine

EOT

chmod +x ./release/launcher

cp ./third_party/luacpp/build/libluacpp.so ./release/

cp ./third_party/ogre-next/build/Release/lib/libOgreMain.so.2.3.3 ./release/
cp ./third_party/ogre-next/build/Release/lib/libOgreHlmsPbs.so.2.3.3 ./release/
cp ./third_party/ogre-next/build/Release/lib/libOgreHlmsUnlit.so.2.3.3 ./release/

cp ./third_party/ogre-next/build/Release/lib/RenderSystem_GL3Plus.so.2.3.3 ./release/
cp ./third_party/ogre-next/build/Release/lib/Plugin_ParticleFX.so.2.3.3 ./release/

cp ./third_party/ogre-next/build/Release/bin/OgreMeshTool ./release/

cp -r ./Component/ ./release/Component/
cp -r ./Core/ ./release/Core/

sudo cp /usr/local/lib/libOIS.so.1.5.1 ./release/

cat <<EOT > ./release/plugins.cfg
# Defines plugins to load

# Define plugin folder
PluginFolder=.

# Define plugins
PluginOptional=RenderSystem_GL3Plus
Plugin=Plugin_ParticleFX

EOT

rm -rf ./release/Media/
mkdir -p ./release/Media/
cp -r ./third_party/ogre-next/Samples/Media/Hlms ./release/Media/Hlms

cp ./cmake-build-release/AmorphousEngine ./release/

cp -r ./projects/ ./release/projects/

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

rm -f "./release-${OS_NAME}-${OS_ARCH}.zip"
zip -r "./release-${OS_NAME}-${OS_ARCH}" ./release
