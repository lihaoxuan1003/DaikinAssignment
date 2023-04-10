cmake_minimum_required(VERSION 3.10)

project(gtest)

set(CMAKE_CXX_STANDARD 20)

add_library(gtest STATIC /Users/HLi/googletest/googletest/src/gtest-all.cc DaikinAssignmentTest.cpp DaikinAssignmentTest.h)

target_include_directories(gtest PUBLIC /Users/HLi/googletest/googletest /Users/HLi/googletest/googletest/include)
