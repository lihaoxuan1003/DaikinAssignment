cmake_minimum_required(VERSION 3.25)

project(DaikinAssignment)

set(CMAKE_CXX_STANDARD 20)

set(SRC_FILES
        main.cpp
        Room/Room.cpp
        Radiator/Radiator.cpp
        AirConditioner/AirConditioner.cpp
        TemperatureRegulator/TemperatureRegulator.cpp)

set(HEADER_FILES
        Room/Room.h
        Heater.h
        Cooler.h
        Radiator/Radiator.h
        AirConditioner/AirConditioner.h
        Radiator/RadiatorEnum.h
        AirConditioner/AirConditionerEnum.h
        Room/ThermalExchangeEnum.h
        TemperatureRegulator/TemperatureRegulator.h
        TemperatureRegulator/TemperatureRegulatorEnum.h)

set(TEST_FILES
        DaikinAssignmentTest/DaikinAssignmentTest.cpp TemperatureRegulator/TemperatureRegulatorEnum.cpp)

add_library(DaikinAssignmentLib STATIC ${SRC_FILES} ${HEADER_FILES})

target_include_directories(DaikinAssignmentLib PUBLIC googletest/googletest/include)

add_subdirectory(DaikinAssignmentTest)

add_executable(DaikinAssignment ${SRC_FILES} ${HEADER_FILES} ${TEST_FILES})

target_link_libraries(DaikinAssignment gtest)

