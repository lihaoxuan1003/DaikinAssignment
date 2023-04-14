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
    radiator = make_shared<Radiator>(RadiatorValveSettings::SettingFive);
    airConditioner = make_shared<AirConditioner>(AirConditionerPowerSettings::Medium);

    officeRoom = make_shared<Room>(TEMPERATURE_MEDIUM);
    officeRoom->addHeater(radiator);
    officeRoom->addCooler(airConditioner);
    officeRoom->setThermalExchange(ThermalExchange::None);

    temperatureRegulator = make_unique<TemperatureRegulator>(officeRoom, TEMPERATURE_LOW, TEMPERATURE_HIGH);
}

float DaikinAssignmentTest::roundTemperature(float temperature) {
    return static_cast<float>(round(temperature * 10)) / 10;
}

TEST_F(DaikinAssignmentTest, TestNotEnoughHeaterOrCooler) {
    TemperatureRegulator temperatureRegulator = TemperatureRegulator(make_shared<Room>(TEMPERATURE_MEDIUM), TEMPERATURE_LOW, TEMPERATURE_HIGH);
    EXPECT_THROW(temperatureRegulator.temperatureRegulationOn(), invalid_argument);
}

TEST_F(DaikinAssignmentTest, TestThermalExchange) {
    ThermalExchange thermalExchange = ThermalExchange::NegativeHigh;
    officeRoom->setThermalExchange(thermalExchange);
    officeRoom->setCurrentTemperature(TEMPERATURE_MEDIUM);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR; i++) {
        officeRoom->applyThermalExchange();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(), roundTemperature(TEMPERATURE_MEDIUM + NUMBER_OF_REGULATIONS_PER_HOUR * thermalExchangeValues[static_cast<size_t>(thermalExchange)]));

    thermalExchange = ThermalExchange::PositiveMedium;
    officeRoom->setThermalExchange(thermalExchange);
    officeRoom->setCurrentTemperature(TEMPERATURE_MEDIUM);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR; i++) {
        officeRoom->applyThermalExchange();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(), roundTemperature(TEMPERATURE_MEDIUM + NUMBER_OF_REGULATIONS_PER_HOUR * thermalExchangeValues[static_cast<size_t>(thermalExchange)]));
    officeRoom->setThermalExchange(ThermalExchange::None);
}

TEST_F(DaikinAssignmentTest, TestHeating) {
    officeRoom->setCurrentTemperature(TEMPERATURE_LOW - 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(), TEMPERATURE_LOW - 5);

    temperatureRegulator->temperatureRegulationOn();
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 2; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(), TEMPERATURE_LOW);
    officeRoom->setCurrentTemperature(TEMPERATURE_LOW - 2);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 2; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(), TEMPERATURE_LOW);

    temperatureRegulator->temperatureRegulationOff();
    officeRoom->setCurrentTemperature(TEMPERATURE_LOW - 10);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 2; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(), TEMPERATURE_LOW - 10);
}

TEST_F(DaikinAssignmentTest, TestHeatingWithAirConditioner) {
    officeRoom->removeHeater(radiator);
    officeRoom->addHeater(airConditioner);

    officeRoom->setCurrentTemperature(TEMPERATURE_LOW - 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(), TEMPERATURE_LOW - 5);

    temperatureRegulator->temperatureRegulationOn();
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 4; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_GT(officeRoom->getCurrentTemperature(), TEMPERATURE_LOW);
    officeRoom->setCurrentTemperature(TEMPERATURE_LOW - 2);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 2; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_GT(officeRoom->getCurrentTemperature(), TEMPERATURE_LOW);

    temperatureRegulator->temperatureRegulationOff();
    officeRoom->setCurrentTemperature(TEMPERATURE_LOW - 10);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 2; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(), TEMPERATURE_LOW - 10);

    officeRoom->removeHeater(airConditioner);
    officeRoom->addHeater(radiator);
}

TEST_F(DaikinAssignmentTest, TestHeatingWithThermalExchangeLow) {
    ThermalExchange thermalExchange = ThermalExchange::NegativeLow;
    officeRoom->setThermalExchange(thermalExchange);
    officeRoom->setCurrentTemperature(TEMPERATURE_LOW);
    temperatureRegulator->setMinTemperature(TEMPERATURE_MEDIUM);
    temperatureRegulator->temperatureRegulationOn();
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 3; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_GT(officeRoom->getCurrentTemperature(), TEMPERATURE_MEDIUM);

    thermalExchange = ThermalExchange::PositiveLow;
    officeRoom->setThermalExchange(thermalExchange);
    officeRoom->setCurrentTemperature(TEMPERATURE_MEDIUM);
    temperatureRegulator->setMaxTemperature(TEMPERATURE_LOW);
    temperatureRegulator->temperatureRegulationOn();
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_LT(officeRoom->getCurrentTemperature(), TEMPERATURE_LOW);

    temperatureRegulator->temperatureRegulationOff();
}

TEST_F(DaikinAssignmentTest, TestHeatingWithThermalExchangeVeryHighNotEnoughHeating) {
    ThermalExchange thermalExchange = ThermalExchange::NegativeVeryHigh;
    officeRoom->setThermalExchange(thermalExchange);
    officeRoom->setCurrentTemperature(TEMPERATURE_LOW);
    temperatureRegulator->setMinTemperature(TEMPERATURE_MEDIUM);
    temperatureRegulator->temperatureRegulationOn();
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 3; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_LT(officeRoom->getCurrentTemperature(), TEMPERATURE_LOW);

    temperatureRegulator->temperatureRegulationOff();
}

TEST_F(DaikinAssignmentTest, TestHeatingWithThermalExchangeVeryHighNotEnoughCooling) {
    ThermalExchange thermalExchange = ThermalExchange::PositiveVeryHigh;
    officeRoom->setThermalExchange(thermalExchange);
    officeRoom->setCurrentTemperature(TEMPERATURE_HIGH);
    temperatureRegulator->setMaxTemperature(TEMPERATURE_MEDIUM);
    temperatureRegulator->temperatureRegulationOn();
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 3; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_GT(officeRoom->getCurrentTemperature(), TEMPERATURE_HIGH);

    temperatureRegulator->temperatureRegulationOff();
}

TEST_F(DaikinAssignmentTest, TestDifferetRadiatorValveSettings) {
    temperatureRegulator->temperatureRegulationOn();

    officeRoom->setCurrentTemperature(MIN_ROOM_TEMPERATURE);
    temperatureRegulator->setMinTemperature(MAX_VALID_ADJUSTABLE_TEMPERATURE);
    radiator->setValveSetting(RadiatorValveSettings::SettingOne);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 24; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(),
              radiatorValveTemperatures[static_cast<size_t>(RadiatorValveSettings::SettingOne)]);

    officeRoom->setCurrentTemperature(MIN_ROOM_TEMPERATURE);
    temperatureRegulator->setMinTemperature(MAX_VALID_ADJUSTABLE_TEMPERATURE);
    radiator->setValveSetting(RadiatorValveSettings::SettingFive);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 24; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(), MAX_VALID_ADJUSTABLE_TEMPERATURE);

    temperatureRegulator->temperatureRegulationOff();
}

TEST_F(DaikinAssignmentTest, TestCooling) {
    officeRoom->setCurrentTemperature(TEMPERATURE_HIGH + 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(), TEMPERATURE_HIGH + 5);

    temperatureRegulator->temperatureRegulationOn();
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    officeRoom->setCurrentTemperature(TEMPERATURE_HIGH + 3);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(), TEMPERATURE_HIGH);


    temperatureRegulator->temperatureRegulationOff();
    officeRoom->setCurrentTemperature(TEMPERATURE_HIGH + 10);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 10; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(), TEMPERATURE_HIGH + 10);
}

TEST_F(DaikinAssignmentTest, TestDifferentAirConditionerPowerSettings) {
    temperatureRegulator->temperatureRegulationOn();

    officeRoom->setCurrentTemperature(TEMPERATURE_HIGH);
    temperatureRegulator->setMaxTemperature(MIN_VALID_ADJUSTABLE_TEMPERATURE);
    airConditioner->setPowerSetting(AirConditionerPowerSettings::Eco);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 10; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_GT(officeRoom->getCurrentTemperature(), MIN_VALID_ADJUSTABLE_TEMPERATURE);

    officeRoom->setCurrentTemperature(TEMPERATURE_HIGH);
    temperatureRegulator->setMaxTemperature(MIN_VALID_ADJUSTABLE_TEMPERATURE);
    airConditioner->setPowerSetting(AirConditionerPowerSettings::High);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 10; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(), MIN_VALID_ADJUSTABLE_TEMPERATURE);

    temperatureRegulator->temperatureRegulationOff();
}

TEST_F(DaikinAssignmentTest, TestSetMinTemperature) {
    temperatureRegulator->temperatureRegulationOn();
    officeRoom->setCurrentTemperature(TEMPERATURE_MEDIUM);

    temperatureRegulator->setMinTemperature(TEMPERATURE_MEDIUM + 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(), TEMPERATURE_MEDIUM + 5);

    temperatureRegulator->setMinTemperature(TEMPERATURE_HIGH + 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(), TEMPERATURE_HIGH + 5);

    EXPECT_EQ(temperatureRegulator->getMinTemperature(), TEMPERATURE_HIGH + 5);
    EXPECT_EQ(temperatureRegulator->getMinTemperature(), temperatureRegulator->getMaxTemperature());

    temperatureRegulator->temperatureRegulationOff();
}

TEST_F(DaikinAssignmentTest, TestSetMaxTemperature) {
    temperatureRegulator->temperatureRegulationOn();
    officeRoom->setCurrentTemperature(TEMPERATURE_MEDIUM);

    temperatureRegulator->setMaxTemperature(TEMPERATURE_MEDIUM - 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(), TEMPERATURE_MEDIUM - 5);

    temperatureRegulator->setMaxTemperature(TEMPERATURE_LOW - 5);
    for (int i = 0; i < NUMBER_OF_REGULATIONS_PER_HOUR * 5; i++) {
        officeRoom->applyThermalExchange();
        temperatureRegulator->regulateTemperature();
    }
    EXPECT_EQ(officeRoom->getCurrentTemperature(), TEMPERATURE_LOW - 5);

    EXPECT_EQ(temperatureRegulator->getMaxTemperature(), TEMPERATURE_LOW - 5);
    EXPECT_EQ(temperatureRegulator->getMinTemperature(), temperatureRegulator->getMaxTemperature());

    temperatureRegulator->temperatureRegulationOff();
}

TEST_F(DaikinAssignmentTest, TestTemperaturesInvalid) {
    EXPECT_THROW(make_unique<TemperatureRegulator>(officeRoom, TEMPERATURE_MEDIUM + 5, TEMPERATURE_MEDIUM - 5), invalid_argument);
    EXPECT_THROW(officeRoom->setCurrentTemperature(MIN_ROOM_TEMPERATURE - 1), invalid_argument);
    EXPECT_THROW(officeRoom->setCurrentTemperature(MAX_ROOM_TEMPERATURE + 1), invalid_argument);
    EXPECT_THROW(temperatureRegulator->setMinTemperature(MIN_VALID_ADJUSTABLE_TEMPERATURE - 1), invalid_argument);
    EXPECT_THROW(temperatureRegulator->setMinTemperature(MAX_VALID_ADJUSTABLE_TEMPERATURE + 1), invalid_argument);
    EXPECT_THROW(temperatureRegulator->setMaxTemperature(MIN_VALID_ADJUSTABLE_TEMPERATURE - 1), invalid_argument);
    EXPECT_THROW(temperatureRegulator->setMaxTemperature(MAX_VALID_ADJUSTABLE_TEMPERATURE + 1), invalid_argument);
}
