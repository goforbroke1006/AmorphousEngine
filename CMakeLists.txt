cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 17)

project(AmorphousEngine)

add_executable(${PROJECT_NAME} main.cpp
        src/Application.cpp
        src/Graphics/OgreNext.cpp
        src/Calculation/Lua53.cpp
)

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

file(COPY ./Core DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./projects DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(test)
