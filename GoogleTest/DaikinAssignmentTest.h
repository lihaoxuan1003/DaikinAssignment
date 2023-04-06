//
// Created by haoxuan.li on 4/5/2023.
//

#ifndef DAIKINASSIGNMENT_DAIKINASSIGNMENTTEST_H
#define DAIKINASSIGNMENT_DAIKINASSIGNMENTTEST_H

#include <gtest/gtest.h>
#include "../Room.h"

using namespace std;

class DaikinAssignmentTest : public ::testing::Test {

protected:
    void SetUp() override;
    void TearDown() override;

    Room* officeRoom;
};

#endif //DAIKINASSIGNMENT_DAIKINASSIGNMENTTEST_H
