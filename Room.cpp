//
// Created by haoxuan.li on 4/4/2023.
//

#include <stdexcept>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include "Room.h"

#define FLOATING_OPERATION_PRECISION 0.01

using namespace std;

Room::Room(float currentTemperature, float minTemperature, float maxTemperature) {
    if (minTemperature > maxTemperature)
        throw invalid_argument("The minimum currentTemperature cannot be higher than the maximum currentTemperature.");

    setTemperature(currentTemperature);
    setMinTemperature(minTemperature);
    setMaxTemperature(maxTemperature);
}

void Room::temperatureRegulationOn() {
    if (heaters.empty() || coolers.empty())
            throw invalid_argument("Cannot regulate temperature: room must have at least one heater and one cooler..");

    temperatureRegulationEnabled = true;
}

void Room::temperatureRegulationOff() {
    temperatureRegulationEnabled = false;
}

// assume the temperature is regulated every 10 mins
void Room::regulateTemperature() {
    if (!temperatureRegulationEnabled)
        return;

    if (minTemperature - currentTemperature >= FLOATING_OPERATION_PRECISION)
        notifyHeaters(currentTemperature, minTemperature, maxTemperature);


    if (currentTemperature - maxTemperature >= FLOATING_OPERATION_PRECISION)
        notifyCoolers(currentTemperature, minTemperature, maxTemperature);

    roundTemperature(currentTemperature);
 //   cout << "Temperature regulated current room currentTemperature is: " << temperatureToString(currentTemperature) << endl;
}

float Room::getTemperature() const {
    return currentTemperature;
}

float Room::getMinTemperature() const {
    return minTemperature;
}

float Room::getMaxTemperature() const {
    return maxTemperature;
}

void Room::setTemperature(float temperature) {
    if (temperature < MIN_ROOM_TEMPERATURE || temperature > MAX_ROOM_TEMPERATURE)
        throw invalid_argument("The room currentTemperature has to be a value between " +
                                temperatureToString(MIN_ROOM_TEMPERATURE) + " and " +
                                temperatureToString(MAX_ROOM_TEMPERATURE) + " degrees.");

    roundTemperature(temperature);
    this->currentTemperature = temperature;
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
    throw invalid_argument("The allowed set currentTemperature is between " +
    temperatureToString(MIN_VALID_ADJUSTABLE_TEMPERATURE) + " and " +
    temperatureToString(MAX_VALID_ADJUSTABLE_TEMPERATURE) +
    " degrees.");
}


void Room::roundTemperature(float& temperature) {
    temperature = static_cast<float>(round(temperature * 10)) / 10;
}

string Room::temperatureToString(float temperature) {
    string temperatureString = to_string(temperature);
    return temperatureString.substr(0, temperatureString.find('.') + 2);
}

void Room::addHeater(shared_ptr<Heater> heater) {
    heaters.push_back(heater);
}

void Room::removeHeater(shared_ptr<Heater> heater) {
    heaters.erase(remove_if(heaters.begin(), heaters.end(),
                               [&heater](const auto& obj) { return obj.get() == heater.get(); }),
                heaters.end());

}

void Room::notifyHeaters(float& currentTemperature, float minTemperature, float maxTemperature) {
    for (const auto& heater: heaters)
        heater->update(currentTemperature, minTemperature, maxTemperature);
}

void Room::addCooler(shared_ptr<Cooler> cooler) {
    coolers.push_back(cooler);
}

void Room::removeCooler(shared_ptr<Cooler> cooler) {
    coolers.erase(remove_if(coolers.begin(), coolers.end(),
                            [&cooler](const auto& obj) { return obj.get() == cooler.get(); }),
                  coolers.end());
}

void Room::notifyCoolers(float& currentTemperature, float minTemperature, float maxTemperature) {
    for (const auto& cooler: coolers)
        cooler->update(currentTemperature, minTemperature, maxTemperature);
}

void Room::applyThermalExchange() {
    currentTemperature += thermalExchangeValues[static_cast<size_t>(thermalExchange)];
}

void Room::setThermalExchange(ThermalExchange thermalExchange) {
    this->thermalExchange = thermalExchange;
}

