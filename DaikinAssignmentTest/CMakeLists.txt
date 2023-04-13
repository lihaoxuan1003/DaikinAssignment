cmake_minimum_required(VERSION 3.25)

project(gtest)

set(CMAKE_CXX_STANDARD 20)

add_library(gtest STATIC
        ../googletest/googletest/src/gtest-all.cc
        DaikinAssignmentTest.cpp
        DaikinAssignmentTest.h)

target_include_directories(gtest PUBLIC
        ../googletest/googletest
        ../googletest/googletest/include)

