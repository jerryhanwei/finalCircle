cmake_minimum_required(VERSION 3.17)
project(untitled1 C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled1 main.c)

set(INC_DIR C://Program Files (x86)//Microsoft SDKs//MPI//Include)

set(LINK_DIR C://Program Files (x86)//Microsoft SDKs//MPI//Lib//x64)
#添加头文件搜索路径
include_directories(${INC_DIR})
#添加库文件搜索路径
link_directories(${LINK_DIR})

#用于将当前目录下的所有源文件的名字保存在变量 DIR_SRCS 中
aux_source_directory(. DIR_SRCS)

add_executable(MpiDemo ${DIR_SRCS})
