cmake_minimum_required(VERSION 3.25)

project(gtest)

set(CMAKE_CXX_STANDARD 20)

add_library(gtest STATIC
        C:/Users/haoxuan.li/googletest-1.13.0/googletest/src/gtest-all.cc
        DaikinAssignmentTest.cpp
        DaikinAssignmentTest.h)

target_include_directories(gtest PUBLIC
        C:/Users/haoxuan.li/googletest-1.13.0/googletest
        C:/Users/haoxuan.li/googletest-1.13.0/googletest/include)

