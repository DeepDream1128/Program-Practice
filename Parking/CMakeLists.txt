cmake_minimum_required(VERSION 3.0.0)
project(Parking VERSION 0.1.0)
set(CMAKE_EXPORT_COMPILECOMMANDS ON) 
include(CTest)
enable_testing()

add_executable(Parking main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
