cmake_minimum_required(VERSION 3.25)

project(DaikinAssignment)

set(CMAKE_CXX_STANDARD 20)

set(SRC_FILES
        main.cpp
        Room.cpp
        Radiator.cpp
        AirConditioner.cpp
        )

set(HEADER_FILES
        Room.h
        Heater.h
        Cooler.h
        Radiator.h
        AirConditioner.h
        RadiatorEnum.h
        AirConditionerEnum.h)

set(TEST_FILES
        GoogleTest/DaikinAssignmentTest.cpp
        )

add_library(DaikinAssignmentLib STATIC ${SRC_FILES} ${HEADER_FILES})

target_include_directories(DaikinAssignmentLib PUBLIC /Users/HLi/googletest/googletest/include)

add_subdirectory(GoogleTest)

add_executable(DaikinAssignment ${SRC_FILES} ${HEADER_FILES} ${TEST_FILES})

target_link_libraries(DaikinAssignment gtest)

