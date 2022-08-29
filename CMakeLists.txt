cmake_minimum_required(VERSION 3.0.0)
project(LoadingMaker VERSION 0.1.0)

if(USE_LINUX)
    set(CMAKE_CXX_FLAGS   ${CMAKE_CXX_FLAGS} -pthread) 
endif()

include(CTest)
enable_testing()

add_executable(LoadingMaker main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
