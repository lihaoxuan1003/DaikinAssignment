//
// Created by haoxuan.li on 4/14/2023.
//

#include <cmath>
#include <utility>
#include <iostream>
#include "TemperatureRegulator.h"
#include "Radiator.h"
#include "AirConditioner.h"

#define FLOATING_OPERATION_PRECISION 0.01

TemperatureRegulator::TemperatureRegulator(shared_ptr<Room> room, float minTemperature, float maxTemperature) :
        room(std::move(room)), minTemperature(minTemperature), maxTemperature(maxTemperature) {
    if (minTemperature > maxTemperature)
        throw invalid_argument("The minimum temperature cannot be higher than the maximum temperature.");
    setMinTemperature(minTemperature);
    setMaxTemperature(maxTemperature);
}


void TemperatureRegulator::temperatureRegulationOn() {
    if (room->getHeaters().empty() || room->getCoolers().empty())
        throw invalid_argument("Cannot regulate temperature: room must have at least one heater and one cooler..");

    temperatureRegulationEnabled = true;
}

void TemperatureRegulator::temperatureRegulationOff() {
    temperatureRegulationEnabled = false;
}

// assume the temperature is regulated every 10 mins
void TemperatureRegulator::regulateTemperature() {
    if (!temperatureRegulationEnabled)
        return;

    auto& currentTemperature = room->getCurrentTemperature();

    if (minTemperature - currentTemperature >= FLOATING_OPERATION_PRECISION) {
        if (autoControlEnabled && !autoHeatingSufficient)
            autoAdjustHeaters();
        room->notifyHeaters(currentTemperature);
        if (autoControlEnabled)
            autoHeatingSufficient = isAutoHeatingSufficient();
    }


    if (currentTemperature - maxTemperature >= FLOATING_OPERATION_PRECISION){
        if (autoControlEnabled && !autoCoolingSufficient)
            autoAdjustCoolers();
        room->notifyCoolers(currentTemperature);
        if (autoControlEnabled)
            autoCoolingSufficient = isAutoCoolingSufficient();
    }




    currentTemperature = roundTemperature(currentTemperature);
    //   cout << "Temperature regulated current room currentTemperature is: " << temperatureToString(currentTemperature) << endl;

    previousTemperature = currentTemperature;
}

float TemperatureRegulator::getMinTemperature() const {
    return minTemperature;
}

float TemperatureRegulator::getMaxTemperature() const {
    return maxTemperature;
}

void TemperatureRegulator::setMinTemperature(float minTemperature) {
    isAdjustableTemperatureValid(minTemperature);

    minTemperature = roundTemperature(minTemperature);
    if (minTemperature > this->maxTemperature)
        this->maxTemperature = minTemperature;
    this->minTemperature = minTemperature;
}

void TemperatureRegulator::setMaxTemperature(float maxTemperature) {
    isAdjustableTemperatureValid(maxTemperature);

    maxTemperature = roundTemperature(maxTemperature);
    if (maxTemperature < this->minTemperature)
        this->minTemperature = maxTemperature;
    this->maxTemperature = maxTemperature;
}

void TemperatureRegulator::isAdjustableTemperatureValid(float temperature) {
    if (temperature < MIN_VALID_ADJUSTABLE_TEMPERATURE || temperature > MAX_VALID_ADJUSTABLE_TEMPERATURE)
        throw invalid_argument("The allowed set currentTemperature is between " +
                               temperatureToString(MIN_VALID_ADJUSTABLE_TEMPERATURE) + " and " +
                               temperatureToString(MAX_VALID_ADJUSTABLE_TEMPERATURE) +
                               " degrees.");
}

float TemperatureRegulator::roundTemperature(float temperature) {
    return static_cast<float>(round(temperature * 10)) / 10;
}

string TemperatureRegulator::temperatureToString(float temperature) {
    string temperatureString = to_string(temperature);
    return temperatureString.substr(0, temperatureString.find('.') + 2);
}

const shared_ptr<Room>& TemperatureRegulator::getRoom() const {
    return room;
}

void TemperatureRegulator::setRoom(const shared_ptr<Room>& room) {
    this->room = room;
}

void TemperatureRegulator::autoControlOn() {
    for (const auto& heater: room->getHeaters()) {
        if (auto radiator = dynamic_cast<Radiator *>(heater.get())) {
            heatersSettingsBackup.push(static_cast<int>(radiator->getValveSetting()));
            radiator->setValveSetting(RadiatorValveSettings::SettingZero);
        } else if (auto airConditioner = dynamic_cast<AirConditioner *>(heater.get())) {
            heatersSettingsBackup.push(static_cast<int>(airConditioner->getPowerSetting()));
            airConditioner->setPowerSetting(AirConditionerPowerSettings::Off);
        } else {
            try {
                throw exception();
            } catch (const exception& e) {
                cerr << "Exception: Unexpected heater type." << '\n';
            }
        }
    }

    for (const auto& cooler: room->getCoolers()) {
        if (auto airConditioner = dynamic_cast<AirConditioner *>(cooler.get())) {
            coolersSettingsBackup.push(static_cast<int>(airConditioner->getPowerSetting()));
            airConditioner->setPowerSetting(AirConditionerPowerSettings::Off);
        } else {
            try {
                throw exception();
            } catch (const exception& e) {
                cerr << "Exception: Unexpected cooler type." << '\n';
            }
        }
    }
    autoControlEnabled = true;
    autoHeatingSufficient = false;
    autoCoolingSufficient = false;
}

void TemperatureRegulator::autoControlOff() {
    if (heatersSettingsBackup.empty() || coolersSettingsBackup.empty() ){
        autoControlEnabled = false;
        return;
    }

    for (const auto& heater: room->getHeaters()) {
        if (auto radiator = dynamic_cast<Radiator*>(heater.get())) {
            radiator->setValveSetting(static_cast<RadiatorValveSettings>(heatersSettingsBackup.front()));
            heatersSettingsBackup.pop();
        } else if (auto airConditioner = dynamic_cast<AirConditioner*>(heater.get())) {
            airConditioner->setPowerSetting(static_cast<AirConditionerPowerSettings>(heatersSettingsBackup.front()));
            heatersSettingsBackup.pop();
        } else {
            try {
                throw exception();
            } catch (const exception& e) {
                cerr << "Exception: Unexpected heater type.`" << '\n';
            }
        }
    }

    for (const auto& cooler: room->getCoolers()) {
        if (auto airConditioner = dynamic_cast<AirConditioner*>(cooler.get())) {
            airConditioner->setPowerSetting(static_cast<AirConditionerPowerSettings>(coolersSettingsBackup.front()));
            coolersSettingsBackup.pop();
        } else {
            try {
                throw exception();
            } catch (const exception& e) {
                cerr << "Exception: Unexpected cooler type." << '\n';
            }
        }
    }
    autoControlEnabled = false;
    autoHeatingSufficient = false;
    autoCoolingSufficient = false;
}

void TemperatureRegulator::autoAdjustHeaters() {
    // increase the heating power
    // use all the radiators before air-conditioners
    for (const auto& heater: room->getHeaters()) {
        if (auto radiator = dynamic_cast<Radiator*>(heater.get())) {
            if (radiator->getValveSetting() != RadiatorValveSettings::SettingFive) {
                radiator->setValveSetting(RadiatorValveSettings::SettingFive);
                return;
            }
        }
    }

    for (const auto& heater: room->getHeaters()) {
        if (auto airConditioner = dynamic_cast<AirConditioner*>(heater.get())) {
            if (airConditioner->getPowerSetting() != AirConditionerPowerSettings::High) {
                airConditioner->setPowerSetting(
                        static_cast<AirConditionerPowerSettings>(static_cast<int>(airConditioner->getPowerSetting()) +
                                                                 1));
                return;
            }
        }
    }
}

void TemperatureRegulator::autoAdjustCoolers() {
    for (const auto& cooler: room->getCoolers()) {
        if (auto airConditioner = dynamic_cast<AirConditioner*>(cooler.get())) {
            if (airConditioner->getPowerSetting() != AirConditionerPowerSettings::High) {
                airConditioner->setPowerSetting(
                        static_cast<AirConditionerPowerSettings>(static_cast<int>(airConditioner->getPowerSetting()) +
                                                                 1));
                return;
            }
        }
    }
}

bool TemperatureRegulator::isHeatingPowerAtMaxLevel() {
    for (const auto& heater: room->getHeaters()) {
        if (auto radiator = dynamic_cast<Radiator*>(heater.get())) {
            if (radiator->getValveSetting() != RadiatorValveSettings::SettingFive)
                return false;
        } else if (auto airConditioner = dynamic_cast<AirConditioner*>(heater.get())) {
            if (airConditioner->getPowerSetting() != AirConditionerPowerSettings::High)
                return false;
        } else {
            try {
                throw exception();
            } catch (const exception& e) {
                cerr << "Exception: Unexpected heater type." << '\n';
            }
        }
    }
    return true;
}

bool TemperatureRegulator::isCoolingPowerAtMaxLevel() {
    for (const auto& cooler: room->getCoolers()) {
        if (auto airConditioner = dynamic_cast<AirConditioner*>(cooler.get())) {
            if (airConditioner->getPowerSetting() != AirConditionerPowerSettings::High)
                return false;
        } else {
            try {
                throw exception();
            } catch (const exception& e) {
                cerr << "Exception: Unexpected cooler type." << '\n';
            }
        }
    }
    return true;
}

bool TemperatureRegulator::isAutoHeatingSufficient() {
    if (previousTemperature < room->getCurrentTemperature())
        return true;

    if (room->getThermalExchange()!= ThermalExchange::None && isHeatingPowerAtMaxLevel()){
        try {
            throw exception();
        } catch (const exception& e) {
            cerr << "Not enough heaters to heat up the room." << '\n';
        }
    }
    return false;
}

bool TemperatureRegulator::isAutoCoolingSufficient() {
    if (previousTemperature > room->getCurrentTemperature())
        return true;

    if (room->getThermalExchange()!= ThermalExchange::None && isCoolingPowerAtMaxLevel()){
        try {
            throw exception();
        } catch (const exception& e) {
            cerr << "Not enough coolers to cool down the room." << '\n';
        }
    }
    return false;
}
