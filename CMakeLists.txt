# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("GetLuckyNum")

set(CMAKE_CXX_STANDARD 20)


find_package(libhv CONFIG REQUIRED)
find_package(unofficial-mysql-connector-cpp CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable(${PROJECT_NAME} "src/main.cpp" "src/Core.h" "src/Core.cpp"   "src/Util.h" "src/Util.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::mysql-connector-cpp::connector)
target_link_libraries(${PROJECT_NAME} PRIVATE hv)

