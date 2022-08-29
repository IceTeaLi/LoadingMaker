cmake_minimum_required(VERSION 3.0.0)
project(LoadingMaker VERSION 0.1.0)
IF (CMAKE_SYSTEM_NAME MATCHES "Linux") 
set(CMAKE_CXX_FLAGS   ${CMAKE_CXX_FLAGS} -pthread) 
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux") 
# 可执行文件目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(CTest)
enable_testing()

add_executable(LoadingMaker main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
