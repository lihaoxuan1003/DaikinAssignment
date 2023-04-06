#include <iostream>
#include <string>
#include <cmath>
#include "Room.h"

#include <gtest/gtest.h>
#include "GoogleTest/DaikinAssignmentTest.h"
using namespace std;



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}