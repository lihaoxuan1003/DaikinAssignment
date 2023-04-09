//
// Created by haoxuan.li on 4/5/2023.
//

#include "DaikinAssignmentTest.h"

#define TEMPERATURE 15
#define MIN_TEMPERATURE 10
#define MAX_TEMPERATURE 25

void DaikinAssignmentTest::SetUp() {
    officeRoom = make_unique<Room>(TEMPERATURE, MIN_TEMPERATURE, MAX_TEMPERATURE);
}

TEST_F(DaikinAssignmentTest, TestHeating) {
    officeRoom->setTemperature(MIN_TEMPERATURE - 5);
    EXPECT_EQ(officeRoom->getTemperature(), MIN_TEMPERATURE - 5);
    officeRoom->temperatureRegulationOn();
    EXPECT_EQ(officeRoom->getTemperature(), MIN_TEMPERATURE);
    officeRoom->setTemperature(MIN_TEMPERATURE - 2);
    EXPECT_EQ(officeRoom->getTemperature(), MIN_TEMPERATURE);
    officeRoom->temperatureRegulationOff();
    officeRoom->setTemperature(MIN_TEMPERATURE - 10);
    EXPECT_EQ(officeRoom->getTemperature(), MIN_TEMPERATURE - 10);
}

TEST_F(DaikinAssignmentTest, TestCooling) {
    officeRoom->setTemperature(MAX_TEMPERATURE + 5);
    EXPECT_EQ(officeRoom->getTemperature(), MAX_TEMPERATURE + 5);
    officeRoom->temperatureRegulationOn();
    EXPECT_EQ(officeRoom->getTemperature(), MAX_TEMPERATURE);
    officeRoom->setTemperature(MAX_TEMPERATURE + 3);
    EXPECT_EQ(officeRoom->getTemperature(), MAX_TEMPERATURE);
    officeRoom->temperatureRegulationOff();
    officeRoom->setTemperature(MAX_TEMPERATURE + 10);
    EXPECT_EQ(officeRoom->getTemperature(), MAX_TEMPERATURE + 10);
}

TEST_F(DaikinAssignmentTest, TestSetMinTemperature) {
    officeRoom->temperatureRegulationOn();
    officeRoom->setMinTemperature(TEMPERATURE + 5);
    EXPECT_EQ(officeRoom->getTemperature(), TEMPERATURE + 5);
    officeRoom->setMinTemperature(MAX_TEMPERATURE + 5);
    EXPECT_EQ(officeRoom->getTemperature(), MAX_TEMPERATURE + 5);
    EXPECT_EQ(officeRoom->getMinTemperature(), MAX_TEMPERATURE + 5);
    EXPECT_EQ(officeRoom->getMinTemperature(), officeRoom->getMaxTemperature());
}

TEST_F(DaikinAssignmentTest, TestSetMaxTemperature) {
    officeRoom->temperatureRegulationOn();
    officeRoom->setMaxTemperature(TEMPERATURE - 5);
    EXPECT_EQ(officeRoom->getTemperature(), TEMPERATURE - 5);
    officeRoom->setMaxTemperature(MIN_TEMPERATURE - 5);
    EXPECT_EQ(officeRoom->getTemperature(), MIN_TEMPERATURE - 5);
    EXPECT_EQ(officeRoom->getMinTemperature(), officeRoom->getMaxTemperature());
}

TEST_F(DaikinAssignmentTest, TestTemperaturesInvalid) {
    EXPECT_THROW(make_unique<Room>(TEMPERATURE, TEMPERATURE + 5, TEMPERATURE - 5), invalid_argument);
    EXPECT_THROW(officeRoom->setTemperature(MIN_ROOM_TEMPERATURE - 1), invalid_argument);
    EXPECT_THROW(officeRoom->setTemperature(MAX_ROOM_TEMPERATURE + 1), invalid_argument);
    EXPECT_THROW(officeRoom->setMinTemperature(MIN_VALID_ADJUSTABLE_TEMPERATURE - 1), invalid_argument);
    EXPECT_THROW(officeRoom->setMinTemperature(MAX_VALID_ADJUSTABLE_TEMPERATURE + 1), invalid_argument);
    EXPECT_THROW(officeRoom->setMaxTemperature(MIN_VALID_ADJUSTABLE_TEMPERATURE - 1), invalid_argument);
    EXPECT_THROW(officeRoom->setMaxTemperature(MAX_VALID_ADJUSTABLE_TEMPERATURE + 1), invalid_argument);
}
