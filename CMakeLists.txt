﻿# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("GetLuckyNum")

set(CMAKE_CXX_STANDARD 20)
set(INCLUDE E:/env64/include)
set(LIB E:/env64/lib)

add_definitions(-DGLOG_NO_ABBREVIATED_SEVERITIES -DWIN32_LEAN_AND_MEAN)

include_directories(${INCLUDE})
link_directories(${LIB})

link_libraries(
        libProp
        glog
        hv
        libconfig++
        libconfig
        libcrypto
        libssl
        libSLS
        mysqlcppconn
        mysqlcppconn8)

add_executable(${PROJECT_NAME} main.cpp Core.h Core.cpp  )

target_link_libraries(${PROJECT_NAME} 
        libProp
        glog
        libSLS
        hv
        libconfig++
        libconfig
        libcrypto
        libssl
        mysqlcppconn
        mysqlcppconn8)
