//
// Created by haoxuan.li on 4/14/2023.
//

#include <cmath>
#include <utility>
#include <iostream>
#include "TemperatureRegulator.h"
#include "../Radiator/Radiator.h"
#include "../AirConditioner/AirConditioner.h"

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
        if (regulationMode == TemperatureRegulationModes::AutoControl && !autoHeatingSufficient)
            autoAdjustHeaters();
        room->notifyHeaters(currentTemperature);
        autoHeatingSufficient = isHeatingSufficient();
    }


    if (currentTemperature - maxTemperature >= FLOATING_OPERATION_PRECISION) {
        if (regulationMode == TemperatureRegulationModes::AutoControl && !autoCoolingSufficient)
            autoAdjustCoolers();
        room->notifyCoolers(currentTemperature);
        autoCoolingSufficient = isCoolingSufficient();
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

void TemperatureRegulator::backupUserSettings() {
    for (const auto& heater: room->getHeaters()) {
        heatersSettingsBackup.push(heater->getHeaterSetting());
        heater->setHeaterSetting(OFF);
    }

    for (const auto& cooler: room->getCoolers()) {
        coolersSettingsBackup.push(cooler->getCoolerSetting());
        cooler->setCoolerSetting(OFF);
    }
}

void TemperatureRegulator::restoreUserSettings() {
    if (heatersSettingsBackup.empty() || coolersSettingsBackup.empty())
        return;


    for (const auto& heater: room->getHeaters()) {
        heater->setHeaterSetting(heatersSettingsBackup.front());
        heatersSettingsBackup.pop();
    }

    for (const auto& cooler: room->getCoolers()) {
        cooler->setCoolerSetting(coolersSettingsBackup.front());
        coolersSettingsBackup.pop();
    }
}

void TemperatureRegulator::autoAdjustHeaters() {
    // increase the heating power
    // use all the radiators before air-conditioners
    for (const auto& heater: room->getHeaters()) {
        auto currentSetting = heater->getHeaterSetting();
        if (currentSetting < heater->getNumberOfHeaterSettings() - 1) {
            heater->setHeaterSetting(currentSetting + 1);
            return;
        }
    }
}

void TemperatureRegulator::autoAdjustCoolers() {
    for (const auto& cooler: room->getCoolers()) {
        auto currentSetting = cooler->getCoolerSetting();
        if (currentSetting < cooler->getNumberOfCoolerSettings() - 1) {
            cooler->setCoolerSetting(currentSetting + 1);
            return;
        }
    }
}

bool TemperatureRegulator::isHeatingPowerAtMaxLevel() {
    for (const auto& heater: room->getHeaters()) {
        if (heater->getHeaterSetting() < heater->getNumberOfHeaterSettings() - 1)
            return false;
    }
    return true;
}

bool TemperatureRegulator::isCoolingPowerAtMaxLevel() {
    for (const auto& cooler: room->getCoolers()) {
        if (cooler->getCoolerSetting() < cooler->getNumberOfCoolerSettings() - 1)
            return false;
    }
    return true;
}

bool TemperatureRegulator::isHeatingSufficient() {
    if (room->getThermalExchange() == ThermalExchange::None || previousTemperature < room->getCurrentTemperature())
        return true;

    if (regulationMode == TemperatureRegulationModes::UserControl) {
        try {
            throw exception();
        } catch (const exception& e) {
            cerr << "Not enough heating power to heat up the room. Adjust the heater settings." << '\n';
        }
    } else if (isHeatingPowerAtMaxLevel()) {
        try {
            throw exception();
        } catch (const exception& e) {
            cerr << "Not enough heaters to heat up the room." << '\n';
        }
    }

    return false;
}

bool TemperatureRegulator::isCoolingSufficient() {
    if (room->getThermalExchange() == ThermalExchange::None || previousTemperature > room->getCurrentTemperature())
        return true;

    if (regulationMode == TemperatureRegulationModes::UserControl) {
        try {
            throw exception();
        } catch (const exception& e) {
            cerr << "Not enough cooling power to cool down the room. Adjust the cooler settings." << '\n';
        }
    } else if (isCoolingPowerAtMaxLevel()) {
        try {
            throw exception();
        } catch (const exception& e) {
            cerr << "Not enough coolers to cool down the room." << '\n';
        }
    }

    return false;
}

TemperatureRegulationModes TemperatureRegulator::getRegulationMode() const {
    return regulationMode;
}

void TemperatureRegulator::setRegulationMode(TemperatureRegulationModes regulationMode) {
    switch (regulationMode) {
        case TemperatureRegulationModes::UserControl:
            if (this->regulationMode != TemperatureRegulationModes::UserControl)
                restoreUserSettings();
            break;
        case TemperatureRegulationModes::AutoControl:
            if (this->regulationMode == TemperatureRegulationModes::UserControl)
                backupUserSettings();
            autoHeatingSufficient = false;
            autoCoolingSufficient = false;
            break;
        case TemperatureRegulationModes::Turbo:
            if (this->regulationMode == TemperatureRegulationModes::UserControl)
                backupUserSettings();
            turboAdjustHeaters();
            turboAdjustCoolers();
            break;
        default:
            cout << "Temperature regulation mode: " << endl;

    }

    this->regulationMode = regulationMode;
}

void TemperatureRegulator::turboAdjustHeaters() {
    for (const auto& heater: room->getHeaters()) {
        heater->setHeaterSetting(heater->getNumberOfHeaterSettings() - 1);
    }
}

void TemperatureRegulator::turboAdjustCoolers() {
    for (const auto& cooler: room->getCoolers()) {
        cooler->setCoolerSetting(cooler->getNumberOfCoolerSettings() - 1);
    }
}
