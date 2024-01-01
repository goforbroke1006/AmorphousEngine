project(${PROJECT_NAME}-test)

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

#

add_subdirectory(./Core)
