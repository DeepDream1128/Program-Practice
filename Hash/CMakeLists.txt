cmake_minimum_required(VERSION 3.0.0)
project(StudentVotingSystem VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(StudentVotingSystem main.cpp student.cpp)

target_include_directories(StudentVotingSystem PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
