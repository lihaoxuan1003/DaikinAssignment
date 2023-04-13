//
// Created by haoxuan.li on 4/5/2023.
//

#ifndef DAIKINASSIGNMENT_DAIKINASSIGNMENTTEST_H
#define DAIKINASSIGNMENT_DAIKINASSIGNMENTTEST_H

#include <gtest/gtest.h>
#include "../Room.h"
#include "../Radiator.h"
#include "../AirConditioner.h"

using namespace std;

class DaikinAssignmentTest : public ::testing::Test {

protected:
    void SetUp() override;
    float roundTemperature(float temperature);

    unique_ptr<Room> officeRoom;
    shared_ptr<Radiator> radiator;
    shared_ptr<AirConditioner> airConditioner;
};

#endif //DAIKINASSIGNMENT_DAIKINASSIGNMENTTEST_H
