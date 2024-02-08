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
PROJECT_DIR="\$(pwd)"
cd "\${SCRIPT_DIR}"

LD_LIBRARY_PATH=. ./AmorphousEngine OgreNext "\${SCRIPT_DIR}" "\${PROJECT_DIR}" "\${PROJECT_DIR}/Scenes/level-0.json"

EOT

chmod +x ./release/launcher

cp ./third_party/luacpp/build/libluacpp.so ./release/

cp ~/ogre/build/Release/lib/libOgreMain.so.14.1 ./release/
cp ~/ogre/build/Release/lib/libOgreBites.so.14.1 ./release/
cp ~/ogre/build/Release/lib/libOgreRTShaderSystem.so.14.1 ./release/
#cp ./third_party/ogre-next/build/Release/lib/libOgreMain.so.2.3.3 ./release/
#cp ./third_party/ogre-next/build/Release/lib/libOgreHlmsPbs.so.2.3.3 ./release/
#cp ./third_party/ogre-next/build/Release/lib/libOgreHlmsUnlit.so.2.3.3 ./release/

cp ~/ogre/build/Release/lib/RenderSystem_GL.so.14.1 ./release/
cp ~/ogre/build/Release/lib/RenderSystem_GL3Plus.so.14.1 ./release/
cp ~/ogre/build/Release/lib/RenderSystem_GLES2.so.14.1 ./release/
cp ~/ogre/build/Release/lib/Plugin_ParticleFX.so.14.1 ./release/
cp ~/ogre/build/Release/lib/Plugin_BSPSceneManager.so.14.1 ./release/
cp ~/ogre/build/Release/lib/Codec_STBI.so.14.1 ./release/
#cp ./third_party/ogre-next/build/Release/lib/RenderSystem_GL3Plus.so.2.3.3 ./release/
#cp ./third_party/ogre-next/build/Release/lib/Plugin_ParticleFX.so.2.3.3 ./release/

#cp ./third_party/ogre-next/build/Release/bin/OgreMeshTool ./release/

rm -rf release/Component
cp -r ./Component/ ./release/

rm -rf release/Core
cp -r ./Core/ ./release/

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

rm -f "./release-${OS_NAME}-${OS_ARCH}.zip"
zip -r "./release-${OS_NAME}-${OS_ARCH}" ./release
