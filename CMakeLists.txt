cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 17)

project(AmorphousEngine)

add_executable(${PROJECT_NAME} main.cpp
        src/Core/Color.cpp
        src/Core/GameObject.cpp
        src/Core/KeyCode.cpp
        src/Application.cpp
        src/Graphics/OgreOne.cpp
        src/Graphics/Irrlicht.cpp
        src/Calculation/Lua53.cpp
        src/Calculation/Lua53Generator.cpp
        src/GraphicsEngine.cpp
        src/InputReaderOIS.cpp
        src/SceneState.cpp
        src/PrefabManager.cpp
)

# Locate Ogre3D
find_package(OGRE 14.1.0 REQUIRED COMPONENTS Bites RTShaderSystem)
target_link_libraries(${PROJECT_NAME} OgreBites OgreRTShaderSystem)

# Locate LuaCpp
find_package(LuaCpp REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR} ${LUACPP_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${LUA_LIBRARIES} ${LUACPP_LIBRARIES})

# Locate JsonCpp
find_package(jsoncpp REQUIRED)
target_link_libraries(${PROJECT_NAME} jsoncpp)

#find_package(irrlicht REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE "/usr/include/irrlicht")
target_link_libraries(${PROJECT_NAME} "/usr/lib/x86_64-linux-gnu/libIrrlicht.so")

# OIS
target_link_libraries(${PROJECT_NAME} OIS)

file(COPY ./Core DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./Component DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./projects DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ./plugins.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#
# GTEST
#
set(GTEST_VERSION 1.14.0)
set(GTEST_ARCHIVE_FILENAME googletest-v${GTEST_VERSION}.tar.gz)
set(GTEST_ARCHIVE_FULL_PATH ${PROJECT_BINARY_DIR}/${GTEST_ARCHIVE_FILENAME})
set(GTEST_BUILD_PATH ${PROJECT_BINARY_DIR}/googletest-${GTEST_VERSION})
#
# Download release from https://github.com/google/googletest/ if TAR.GZ not found locally
#
if (NOT EXISTS ${GTEST_ARCHIVE_FULL_PATH})
    set(GTEST_RELEASE_ARCHIVE_URL https://github.com/google/googletest/archive/refs/tags/v${GTEST_VERSION}.tar.gz)

    message(STATUS "Download " ${GTEST_RELEASE_ARCHIVE_URL})
    file(DOWNLOAD ${GTEST_RELEASE_ARCHIVE_URL} ${GTEST_ARCHIVE_FULL_PATH})
    file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/googletest-${GTEST_VERSION})
endif ()
#
# Build GTest
#
message(STATUS "Building GTest ${GTEST_VERSION}")
execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${GTEST_ARCHIVE_FILENAME} WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
execute_process(COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} ${GTEST_BUILD_PATH} -Dgtest_force_shared_crt=TRUE -DBUILD_GMOCK=FALSE
        WORKING_DIRECTORY ${GTEST_BUILD_PATH})
execute_process(COMMAND ${CMAKE_COMMAND} --build ${GTEST_BUILD_PATH})

#
# Add includes, sources and libraries for GTest
#
set(GTEST_INCLUDE_DIR ${GTEST_BUILD_PATH}/googletest/include)
set(gtest_SOURCE_DIR ${GTEST_BUILD_PATH}/googletest/src)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(GTEST_LIBRARY ${GTEST_BUILD_PATH}/lib/libgtest.a)
    set(GTEST_MAIN_LIBRARY ${GTEST_BUILD_PATH}/lib/libgtest_main.a)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(GTEST_LIBRARY ${GTEST_BUILD_PATH}/lib/gtest.lib)
    set(GTEST_MAIN_LIBRARY ${GTEST_BUILD_PATH}/lib/gtest_main.lib)
endif ()

#
enable_testing()

add_subdirectory(test)
