cmake_minimum_required(VERSION 3.25)
project(DaikinAssignment)

set(CMAKE_CXX_STANDARD 20)

include_directories(C:/Users/haoxuan.li/googletest-1.13.0/googletest/include)

add_subdirectory(GoogleTest)

add_executable(DaikinAssignment main.cpp Room.cpp Room.h GoogleTest/DaikinAssignmentTest.cpp GoogleTest/DaikinAssignmentTest.h)

target_link_libraries(DaikinAssignment gtest)