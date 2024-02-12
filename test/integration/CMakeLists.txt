project(${PROJECT_NAME}-integration)

add_compile_definitions(DEBUG "")

file(GLOB test_source_files
        "./*.cxx" "./*.cpp"
        "./*/*.cxx" "./*/*.cpp"
)
add_executable(${PROJECT_NAME}
        ../../src/Calculation/Lua53.cpp
        ../../src/Calculation/Lua53Generator.cpp

        ../../src/Core/Color.cpp
        ../../src/Core/KeyCode.cpp
        ../../src/Core/GameObject.cpp

        ../../src/Graphics/OgreOne.cpp

        ../../src/Application.cpp
        ../../src/GraphicsEngine.cpp
        ../../src/PrefabManager.cpp
        ../../src/SceneState.cpp
        ${test_source_files})
add_test(AllTests-${PROJECT_NAME} ${PROJECT_NAME})

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ${gtest_SOURCE_DIR})
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} pthread)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES})
endif ()

# Locate Ogre3D
find_package(OGRE 14.1.0 REQUIRED COMPONENTS Bites RTShaderSystem)
target_link_libraries(${PROJECT_NAME} OgreBites OgreRTShaderSystem)

# OIS
target_link_libraries(${PROJECT_NAME} OIS)

# Locate LuaCpp
find_package(LuaCpp REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR} ${LUACPP_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${LUA_LIBRARIES} ${LUACPP_LIBRARIES})

# Locate JsonCpp
#find_package(jsoncpp REQUIRED)
target_link_libraries(${PROJECT_NAME} jsoncpp)

file(COPY ../../Component DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ../../Core DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ./testdata DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./ogre.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./plugins.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
