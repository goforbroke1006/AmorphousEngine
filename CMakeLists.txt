cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 17)

project(ogrity)

add_executable(${PROJECT_NAME} main.cpp)

find_package(LuaCpp REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE
        ${LUA_INCLUDE_DIR}
        ${LUACPP_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
        ${LUA_LIBRARIES}
        ${LUACPP_LIBRARIES}
)

file(COPY ./Core DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./Scripts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
