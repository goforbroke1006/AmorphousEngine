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

mkdir -p release

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

cp -r ./Component/ ./release/Component/
cp -r ./Core/ ./release/Core/

cat <<EOT > ./release/plugins.cfg
# Defines plugins to load

# Define plugin folder
PluginFolder=.

# Define plugins
PluginOptional=RenderSystem_GL3Plus
Plugin=Plugin_ParticleFX

EOT

cat <<EOT > ./release/resources2.cfg
# Resources required by the sample browser and most samples.
[Essential]
Zip=./Media/packs/DebugPack.zip
Zip=./Media/packs/CornellBox.zip
#Zip=./Media/packs/profiler.zip

# Common sample resources needed by many of the samples.
# Rarely used resources should be separately loaded by the
# samples which require them.
[Popular]
FileSystem=./Media/2.0/scripts/Compositors
FileSystem=./Media/models
FileSystem=./Media/materials/textures
FileSystem=./Media/materials/textures/Cubemaps

[General]
FileSystem=./Media/2.0/scripts/materials/Common
FileSystem=./Media/2.0/scripts/materials/Common/Any
FileSystem=./Media/2.0/scripts/materials/Common/GLSL
FileSystem=./Media/2.0/scripts/materials/Common/GLSLES
FileSystem=./Media/2.0/scripts/materials/Common/HLSL
FileSystem=./Media/2.0/scripts/materials/Common/Metal
FileSystem=./Media/Hlms/Common/Any
FileSystem=./Media/Hlms/Common/GLSL
FileSystem=./Media/Hlms/Common/HLSL
FileSystem=./Media/Hlms/Common/Metal
FileSystem=./Media/Compute/Algorithms/IBL
FileSystem=./Media/Compute/Tools/Any

# Do not load this as a resource. It's here merely to tell the code where
# the Hlms templates are located
[Hlms]
DoNotUseAsResource=./Media

EOT

cp -r ./third_party/ogre-next/Samples/Media/ ./release/Media/

cp ./cmake-build-release/AmorphousEngine ./release/

cp -r ./projects/ ./release/projects/
