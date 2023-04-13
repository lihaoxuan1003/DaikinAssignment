//
// Created by haoxuan.li on 4/5/2023.
//

#include "DaikinAssignmentTest.h"

#define TEMPERATURE_LOW 10
#define TEMPERATURE_MEDIUM 15
#define TEMPERATURE_HIGH 25

#define NUMBER_OF_REGULATIONS_PER_HOUR 6

// assume regulation is triggered every 10 mins

#include <cmath>

void DaikinAssignmentTest::SetUp() {
    officeRoom = make_unique<Room>(TEMPERATURE_MEDIUM, TEMPERATURE_LOW, TEMPERATURE_HIGH);
    radiator = make_shared<Radiator>(RadiatorValveSettings::SettingFive);
    airConditioner = make_shared<AirConditioner>(AirConditionerPowerSettings::Medium);

    officeRoom->addHeater(radiator);
    officeRoom->addCooler(airConditioner);

    officeRoom->setThermalExchange(ThermalExchange::None);
}

float DaikinAssignmentTest::roundTemperature(float temperature) {
    float temp =static_cast<float>(round(temperature * 10)) / 10;
    return temp;
}

TEST_F(DaikinAssignmentTest, TestNotEnoughHeaterOrCooler) {
    unique_ptr<Room> room = make_unique<Room>(TEMPERATURE_MEDIUM, TEMPERATURE_LOW, TEMPERATURE_HIGH);
    EXPECT_THROW(room->temperatureRegulationOn(), invalid_argument);
}

TEST_F(DaikinAssignmentTest, TestThermalExchange) {
    ThermalExchange thermalExchange = ThermalExchange::NegativeHigh;
    officeRoom->setThermalExchange(thermalExchange);
    officeRoom->setTemperature(TEMPERATURE_MEDIUM);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR; i++) {
        officeRoom->applyThermalExchange();
    }
    EXPECT_EQ(officeRoom->getTemperature(), roundTemperature(TEMPERATURE_MEDIUM + NUMBER_OF_REGULATIONS_PER_HOUR * thermalExchangeValues[static_cast<size_t>(thermalExchange)]));

    thermalExchange = ThermalExchange::PositiveMedium;
    officeRoom->setThermalExchange(thermalExchange);
    officeRoom->setTemperature(TEMPERATURE_MEDIUM);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR; i++) {
        officeRoom->applyThermalExchange();
    }
    EXPECT_EQ(officeRoom->getTemperature(), roundTemperature(TEMPERATURE_MEDIUM + NUMBER_OF_REGULATIONS_PER_HOUR * thermalExchangeValues[static_cast<size_t>(thermalExchange)]));
    officeRoom->setThermalExchange(ThermalExchange::None);
}

TEST_F(DaikinAssignmentTest, TestHeating) {
    officeRoom->setTemperature(TEMPERATURE_LOW - 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), TEMPERATURE_LOW - 5);

    officeRoom->temperatureRegulationOn();
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 2; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), TEMPERATURE_LOW);
    officeRoom->setTemperature(TEMPERATURE_LOW - 2);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 2; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), TEMPERATURE_LOW);

    officeRoom->temperatureRegulationOff();
    officeRoom->setTemperature(TEMPERATURE_LOW - 10);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 2; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), TEMPERATURE_LOW - 10);
}

TEST_F(DaikinAssignmentTest, TestHeatingWithThermalExchangeLow) {
    ThermalExchange thermalExchange = ThermalExchange::NegativeLow;
    officeRoom->setThermalExchange(thermalExchange);
    officeRoom->setTemperature(TEMPERATURE_LOW);
    officeRoom->setMinTemperature(TEMPERATURE_MEDIUM);
    officeRoom->temperatureRegulationOn();
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 3; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_GT(officeRoom->getTemperature(), TEMPERATURE_MEDIUM);

    thermalExchange = ThermalExchange::PositiveLow;
    officeRoom->setThermalExchange(thermalExchange);
    officeRoom->setTemperature(TEMPERATURE_MEDIUM);
    officeRoom->setMaxTemperature(TEMPERATURE_LOW);
    officeRoom->temperatureRegulationOn();
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 4; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_LT(officeRoom->getTemperature(), TEMPERATURE_LOW);

    officeRoom->temperatureRegulationOff();
}

TEST_F(DaikinAssignmentTest, TestHeatingWithThermalExchangeVeryHighNotEnoughHeating) {
    ThermalExchange thermalExchange = ThermalExchange::NegativeVeryHigh;
    officeRoom->setThermalExchange(thermalExchange);
    officeRoom->setTemperature(TEMPERATURE_LOW);
    officeRoom->setMinTemperature(TEMPERATURE_MEDIUM);
    officeRoom->temperatureRegulationOn();
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 3; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_LT(officeRoom->getTemperature(), TEMPERATURE_LOW);

    officeRoom->temperatureRegulationOff();
}

TEST_F(DaikinAssignmentTest, TestHeatingWithThermalExchangeVeryHighNotEnoughCooling) {
    ThermalExchange thermalExchange = ThermalExchange::PositiveVeryHigh;
    officeRoom->setThermalExchange(thermalExchange);
    officeRoom->setTemperature(TEMPERATURE_HIGH);
    officeRoom->setMaxTemperature(TEMPERATURE_MEDIUM);
    officeRoom->temperatureRegulationOn();
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 3; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_GT(officeRoom->getTemperature(), TEMPERATURE_HIGH);

    officeRoom->temperatureRegulationOff();
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

    officeRoom->temperatureRegulationOff();
}

TEST_F(DaikinAssignmentTest, TestCooling) {
    officeRoom->setTemperature(TEMPERATURE_HIGH + 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), TEMPERATURE_HIGH + 5);

    officeRoom->temperatureRegulationOn();
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    officeRoom->setTemperature(TEMPERATURE_HIGH + 3);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), TEMPERATURE_HIGH);


    officeRoom->temperatureRegulationOff();
    officeRoom->setTemperature(TEMPERATURE_HIGH + 10);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 10; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), TEMPERATURE_HIGH + 10);
}

TEST_F(DaikinAssignmentTest, TestDifferentAirConditionerPowerSettings) {
    officeRoom->temperatureRegulationOn();

    officeRoom->setTemperature(TEMPERATURE_HIGH);
    officeRoom->setMaxTemperature(MIN_VALID_ADJUSTABLE_TEMPERATURE);
    airConditioner->setPowerSetting(AirConditionerPowerSettings::Eco);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 10; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_GT(officeRoom->getTemperature(), MIN_VALID_ADJUSTABLE_TEMPERATURE);

    officeRoom->setTemperature(TEMPERATURE_HIGH);
    officeRoom->setMaxTemperature(MIN_VALID_ADJUSTABLE_TEMPERATURE);
    airConditioner->setPowerSetting(AirConditionerPowerSettings::High);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 10; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), MIN_VALID_ADJUSTABLE_TEMPERATURE);

    officeRoom->temperatureRegulationOff();
}

TEST_F(DaikinAssignmentTest, TestSetMinTemperature) {
    officeRoom->temperatureRegulationOn();
    officeRoom->setTemperature(TEMPERATURE_MEDIUM);

    officeRoom->setMinTemperature(TEMPERATURE_MEDIUM + 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), TEMPERATURE_MEDIUM + 5);

    officeRoom->setMinTemperature(TEMPERATURE_HIGH + 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), TEMPERATURE_HIGH + 5);

    EXPECT_EQ(officeRoom->getMinTemperature(), TEMPERATURE_HIGH + 5);
    EXPECT_EQ(officeRoom->getMinTemperature(), officeRoom->getMaxTemperature());

    officeRoom->temperatureRegulationOff();
}

TEST_F(DaikinAssignmentTest, TestSetMaxTemperature) {
    officeRoom->temperatureRegulationOn();
    officeRoom->setTemperature(TEMPERATURE_MEDIUM);

    officeRoom->setMaxTemperature(TEMPERATURE_MEDIUM - 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), TEMPERATURE_MEDIUM - 5);

    officeRoom->setMaxTemperature(TEMPERATURE_LOW - 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        officeRoom->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getTemperature(), TEMPERATURE_LOW - 5);

    EXPECT_EQ(officeRoom->getMaxTemperature(), TEMPERATURE_LOW - 5);
    EXPECT_EQ(officeRoom->getMinTemperature(), officeRoom->getMaxTemperature());

    officeRoom->temperatureRegulationOff();
}

TEST_F(DaikinAssignmentTest, TestTemperaturesInvalid) {
    EXPECT_THROW(make_unique<Room>(TEMPERATURE_MEDIUM, TEMPERATURE_MEDIUM + 5, TEMPERATURE_MEDIUM - 5), invalid_argument);
    EXPECT_THROW(officeRoom->setTemperature(MIN_ROOM_TEMPERATURE - 1), invalid_argument);
    EXPECT_THROW(officeRoom->setTemperature(MAX_ROOM_TEMPERATURE + 1), invalid_argument);
    EXPECT_THROW(officeRoom->setMinTemperature(MIN_VALID_ADJUSTABLE_TEMPERATURE - 1), invalid_argument);
    EXPECT_THROW(officeRoom->setMinTemperature(MAX_VALID_ADJUSTABLE_TEMPERATURE + 1), invalid_argument);
    EXPECT_THROW(officeRoom->setMaxTemperature(MIN_VALID_ADJUSTABLE_TEMPERATURE - 1), invalid_argument);
    EXPECT_THROW(officeRoom->setMaxTemperature(MAX_VALID_ADJUSTABLE_TEMPERATURE + 1), invalid_argument);
}
