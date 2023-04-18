//
// Created by haoxuan.li on 4/5/2023.
//

#ifndef DAIKINASSIGNMENT_DAIKINASSIGNMENTTEST_H
#define DAIKINASSIGNMENT_DAIKINASSIGNMENTTEST_H

#include <gtest/gtest.h>
#include "../Room/Room.h"
#include "../Radiator/Radiator.h"
#include "../AirConditioner/AirConditioner.h"
#include "../TemperatureRegulator/TemperatureRegulator.h"

using namespace std;

class DaikinAssignmentTest : public ::testing::Test {

protected:
    void SetUp() override;
    float roundTemperature(float temperature);

    unique_ptr<TemperatureRegulator> temperatureRegulator;
    shared_ptr<Room> officeRoom;
    shared_ptr<Radiator> radiator;
    shared_ptr<AirConditioner> airConditioner;
};

#endif //DAIKINASSIGNMENT_DAIKINASSIGNMENTTEST_H
