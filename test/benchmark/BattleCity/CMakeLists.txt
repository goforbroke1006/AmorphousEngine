project(${PROJECT_NAME}-benchmark-BattleCity)

add_compile_definitions(DEBUG "")

file(GLOB test_source_files
        "./*.cxx" "./*.cpp"
        "./*/*.cxx" "./*/*.cpp"
)
add_executable(${PROJECT_NAME} ./main.cpp

        ../../../src/Calculation/Lua53.cpp
        ../../../src/Calculation/Lua53Generator.cpp

        ../../../src/Core/Color.cpp
        ../../../src/Core/KeyCode.cpp
        ../../../src/Core/GameObject.cpp

        ../../../src/Graphics/Null.cpp

        ../../../src/Application.cpp
        ../../../src/GraphicsEngine.cpp
        ../../../src/PrefabManager.cpp
        ../../../src/SceneState.cpp

        ${test_source_files})

find_package(benchmark REQUIRED)
target_link_libraries(${PROJECT_NAME} benchmark::benchmark)

## Locate Ogre3D
#find_package(OGRE 14.1.0 REQUIRED COMPONENTS Bites RTShaderSystem)
#target_link_libraries(${PROJECT_NAME} OgreBites OgreRTShaderSystem)

# Locate LuaCpp
find_package(LuaCpp REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR} ${LUACPP_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${LUA_LIBRARIES} ${LUACPP_LIBRARIES})

# Locate JsonCpp
#find_package(jsoncpp REQUIRED)
target_link_libraries(${PROJECT_NAME} jsoncpp)

file(COPY ../../../Component DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ../../../Core DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ./testdata DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./ogre.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./plugins.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
