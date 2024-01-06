cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 17)

project(AmorphousEngine)

add_executable(${PROJECT_NAME} main.cpp
        src/Application.cpp
        src/Graphics/OgreNext/OgreNext.cpp
        src/Graphics/Irrlicht.cpp
        src/Calculation/Lua53.cpp
        src/Color.cpp
        src/GameObject.cpp
)

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}			-DDEBUG=1 -D_DEBUG=1 -g")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}		-Os -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}			-O2 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}	-O2 -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-overloaded-virtual")
endif ()
if (LINUX)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/OGRE/cmake)
endif (LINUX)

find_package(OGRE 2.3.3 REQUIRED COMPONENTS Bites RTShaderSystem)
target_include_directories(${PROJECT_NAME} PRIVATE
        ${OGRE_INCLUDE_DIRS}
        ${OGRE_INCLUDE_DIRS}/Hlms
        ${OGRE_INCLUDE_DIRS}/Hlms/Common
        ${OGRE_HlmsPbs_INCLUDE_DIRS}
        ${OGRE_HlmsUnlit_INCLUDE_DIRS}
        "${OGRE_HlmsUnlit_INCLUDE_DIRS}/../Common"
)
target_link_libraries(${PROJECT_NAME}
        ${OGRE_LIBRARIES}
        ${OGRE_HlmsPbs_LIBRARIES}
        ${OGRE_HlmsUnlit_LIBRARIES})

find_package(LuaCpp REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE
        ${LUA_INCLUDE_DIR}
        ${LUACPP_INCLUDE_DIR}
)
target_link_libraries(${PROJECT_NAME}
        ${LUA_LIBRARIES}
        ${LUACPP_LIBRARIES}
)

find_package(jsoncpp REQUIRED)
target_link_libraries(${PROJECT_NAME} jsoncpp)

#find_package(irrlicht REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE "/usr/include/irrlicht")
target_link_libraries(${PROJECT_NAME} "/usr/lib/x86_64-linux-gnu/libIrrlicht.so")

file(COPY ./Core DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./Component DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./projects DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

IF (CMAKE_BUILD_TYPE MATCHES Debug)
    file(COPY ./third_party/ogre-next/build/Debug/bin/plugins_d.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ./third_party/ogre-next/build/Debug/bin/resources2.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)
IF (CMAKE_BUILD_TYPE MATCHES Release)
    file(COPY ./third_party/ogre-next/build/Release/bin/plugins.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ./third_party/ogre-next/build/Release/bin/resources2.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ENDIF (CMAKE_BUILD_TYPE MATCHES Release)

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
