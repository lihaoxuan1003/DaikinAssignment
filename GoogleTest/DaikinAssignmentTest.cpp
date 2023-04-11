//
// Created by haoxuan.li on 4/5/2023.
//

#include "DaikinAssignmentTest.h"

#define TEMPERATURE 15
#define MIN_TEMPERATURE 10
#define MAX_TEMPERATURE 25

#define NUMBER_OF_REGULATIONS_PER_HOUR 6

// assume regulation is triggered every 10 mins


void DaikinAssignmentTest::SetUp() {
    officeRoom = make_unique<Room>(TEMPERATURE, MIN_TEMPERATURE, MAX_TEMPERATURE);
    radiator = make_shared<Radiator>(RadiatorValveSettings::SettingFive);
    airConditioner = make_shared<AirConditioner>(AirConditionerPowerSettings::Low);

    officeRoom->addHeater(radiator);
    officeRoom->addCooler(airConditioner);

    officeRoom->setThermalExchange(ThermalExchange::None);
}

TEST_F(DaikinAssignmentTest, TestNotEnoughHeaterOrCooler) {
    unique_ptr<Room> room = make_unique<Room>(TEMPERATURE, MIN_TEMPERATURE, MAX_TEMPERATURE);
    EXPECT_THROW(room->temperatureRegulationOn(), invalid_argument);
}

TEST_F(DaikinAssignmentTest, TestHeating) {
    officeRoom->setTemperature(MIN_TEMPERATURE - 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), MIN_TEMPERATURE - 5);

    officeRoom->temperatureRegulationOn();
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 2; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), MIN_TEMPERATURE);
    officeRoom->setTemperature(MIN_TEMPERATURE - 2);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 2; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), MIN_TEMPERATURE);

    officeRoom->temperatureRegulationOff();
    officeRoom->setTemperature(MIN_TEMPERATURE - 10);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 2; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), MIN_TEMPERATURE - 10);
}

TEST_F(DaikinAssignmentTest, TestDifferetRadiatorValveSettings) {
    officeRoom->temperatureRegulationOn();

    officeRoom->setTemperature(MIN_ROOM_TEMPERATURE);
    officeRoom->setMinTemperature(MAX_VALID_ADJUSTABLE_TEMPERATURE);
    radiator->setValveSetting(RadiatorValveSettings::SettingOne);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 24; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(),
              radiatorValveTemperatures[static_cast<size_t>(RadiatorValveSettings::SettingOne)]);

    officeRoom->setTemperature(MIN_ROOM_TEMPERATURE);
    officeRoom->setMinTemperature(MAX_VALID_ADJUSTABLE_TEMPERATURE);
    radiator->setValveSetting(RadiatorValveSettings::SettingFive);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 24; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), MAX_VALID_ADJUSTABLE_TEMPERATURE);
}

TEST_F(DaikinAssignmentTest, TestCooling) {
    officeRoom->setTemperature(MAX_TEMPERATURE + 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), MAX_TEMPERATURE + 5);

    officeRoom->temperatureRegulationOn();
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    officeRoom->setTemperature(MAX_TEMPERATURE + 3);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), MAX_TEMPERATURE);


    officeRoom->temperatureRegulationOff();
    officeRoom->setTemperature(MAX_TEMPERATURE + 10);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 10; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), MAX_TEMPERATURE + 10);
}

TEST_F(DaikinAssignmentTest, TestDifferentAirConditionerPowerSettings) {
    officeRoom->temperatureRegulationOn();

    officeRoom->setTemperature(MAX_TEMPERATURE);
    officeRoom->setMaxTemperature(MIN_VALID_ADJUSTABLE_TEMPERATURE);
    airConditioner->setPowerSetting(AirConditionerPowerSettings::Eco);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 10; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_GT(officeRoom->getTemperature(), MIN_VALID_ADJUSTABLE_TEMPERATURE);

    officeRoom->setTemperature(MAX_TEMPERATURE);
    officeRoom->setMaxTemperature(MIN_VALID_ADJUSTABLE_TEMPERATURE);
    airConditioner->setPowerSetting(AirConditionerPowerSettings::High);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 10; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), MIN_VALID_ADJUSTABLE_TEMPERATURE);
}

TEST_F(DaikinAssignmentTest, TestSetMinTemperature) {
    officeRoom->temperatureRegulationOn();
    officeRoom->setTemperature(TEMPERATURE);

    officeRoom->setMinTemperature(TEMPERATURE + 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), TEMPERATURE + 5);

    officeRoom->setMinTemperature(MAX_TEMPERATURE + 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), MAX_TEMPERATURE + 5);

    EXPECT_EQ(officeRoom->getMinTemperature(), MAX_TEMPERATURE + 5);
    EXPECT_EQ(officeRoom->getMinTemperature(), officeRoom->getMaxTemperature());
}

TEST_F(DaikinAssignmentTest, TestSetMaxTemperature) {
    officeRoom->temperatureRegulationOn();
    officeRoom->setTemperature(TEMPERATURE);

    officeRoom->setMaxTemperature(TEMPERATURE - 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), TEMPERATURE - 5);

    officeRoom->setMaxTemperature(MIN_TEMPERATURE - 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), MIN_TEMPERATURE - 5);

    EXPECT_EQ(officeRoom->getMaxTemperature(), MIN_TEMPERATURE - 5);
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
