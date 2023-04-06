//
// Created by haoxuan.li on 4/4/2023.
//

#include <stdexcept>
#include <cmath>
#include <iomanip>
#include <iostream>

#include "Room.h"

using namespace std;


Room::Room(float temperature, float minTemperature, float maxTemperature) {
    setTemperature(temperature);
    setMinTemperature(minTemperature);
    setMaxTemperature(maxTemperature);
}

void Room::temperatureRegulationOn() {
    temperatureRegulationEnabled = true;
    regulateTemperature();
}

void Room::temperatureRegulationOff() {
    temperatureRegulationEnabled = false;
}

void Room::regulateTemperature() {
    if (!temperatureRegulationEnabled)
        return;

    if (temperature < minTemperature)
        cout << "Turn heating on..." << endl;
    while (minTemperature - temperature >= 0.01) {
        heat();
    }

    if (temperature > maxTemperature)
        cout << "Turn cooling on..." << endl;
    while (temperature - maxTemperature >= 0.01) {
        cool();
    }

    roundTemperature(temperature);
    cout << "Temperature regulated current room temperature is: " << temperatureToString(temperature) << endl;
}

void Room::heat() {
    temperature += 0.1;
}

void Room::cool() {
    temperature -= 0.1;
}

float Room::getTemperature() const {
    return temperature;
}

float Room::getMinTemperature() const {
    return minTemperature;
}

float Room::getMaxTemperature() const {
    return maxTemperature;
}

void Room::setTemperature(float temperature) {
    if (temperature < MIN_ROOM_TEMPERATURE || temperature > MAX_ROOM_TEMPERATURE)
        throw invalid_argument("The room temperature has to be a value between " +
                                temperatureToString(MIN_ROOM_TEMPERATURE) + " and " +
                                temperatureToString(MAX_ROOM_TEMPERATURE) + " degrees.");

    roundTemperature(temperature);
    this->temperature = temperature;
    regulateTemperature();
}

void Room::setMinTemperature(float minTemperature) {
    isAdjustableTemperatureValid(minTemperature);

    roundTemperature(minTemperature);
    if (minTemperature > this->maxTemperature)
        this->maxTemperature = minTemperature;
    this->minTemperature = minTemperature;
    regulateTemperature();
}

void Room::setMaxTemperature(float maxTemperature) {
    isAdjustableTemperatureValid(maxTemperature);

    roundTemperature(maxTemperature);
    if (maxTemperature < this->minTemperature)
        this->minTemperature = maxTemperature;
    this->maxTemperature = maxTemperature;
    regulateTemperature();
}

void Room::isAdjustableTemperatureValid(float temperature) {
    if (temperature < MIN_VALID_ADJUSTABLE_TEMPERATURE || temperature > MAX_VALID_ADJUSTABLE_TEMPERATURE)
        throw invalid_argument("The allowed set temperature is between " +
                               temperatureToString(MIN_VALID_ADJUSTABLE_TEMPERATURE) + " and " +
                               temperatureToString(MAX_VALID_ADJUSTABLE_TEMPERATURE) +
                               " degrees.");
}

void Room::roundTemperature(float &temperature) {
    temperature = static_cast<float>(round(temperature * 10)) / 10;
}

string Room::temperatureToString(float temperature) {
    string temperatureString = to_string(temperature);
    return temperatureString.substr(0, temperatureString.find('.') + 2);
}


