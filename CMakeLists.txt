cmake_minimum_required(VERSION 3.10)

project(MyProject)

# 添加头文件目录
include_directories(${CMAKE_SOURCE_DIR}/code/include)

# 添加源文件
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/code/*.cpp)

# 生成可执行文件
add_executable(my_webserver ${SRC_FILES})
