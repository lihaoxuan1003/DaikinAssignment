//
// Created by haoxuan.li on 4/4/2023.
//

#include <stdexcept>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include "Room.h"

using namespace std;

Room::Room(float currentTemperature) : currentTemperature(currentTemperature) {}

float& Room::getCurrentTemperature()  {
    return currentTemperature;
}

const vector<shared_ptr<Heater>>& Room::getHeaters() const {
    return heaters;
}

const vector<shared_ptr<Cooler>>& Room::getCoolers() const {
    return coolers;
}

void Room::setCurrentTemperature(float temperature) {
    if (temperature < MIN_ROOM_TEMPERATURE || temperature > MAX_ROOM_TEMPERATURE)
        throw invalid_argument("The room currentTemperature has to be a value between " +
                                temperatureToString(MIN_ROOM_TEMPERATURE) + " and " +
                                temperatureToString(MAX_ROOM_TEMPERATURE) + " degrees.");

    temperature = roundTemperature(temperature);
    this->currentTemperature = temperature;
}

void Room::setThermalExchange(ThermalExchange thermalExchange) {
    this->thermalExchange = thermalExchange;
}

void Room::addHeater(shared_ptr<Heater> heater) {
    heaters.push_back(heater);
}

void Room::removeHeater(shared_ptr<Heater> heater) {
    heaters.erase(remove_if(heaters.begin(), heaters.end(),
                               [&heater](const auto& obj) { return obj.get() == heater.get(); }),
                heaters.end());

}

void Room::notifyHeaters(float& currentTemperature) {
    for (const auto& heater: heaters)
        heater->heat(currentTemperature);
}

void Room::addCooler(shared_ptr<Cooler> cooler) {
    coolers.push_back(cooler);
}

void Room::removeCooler(shared_ptr<Cooler> cooler) {
    coolers.erase(remove_if(coolers.begin(), coolers.end(),
                            [&cooler](const auto& obj) { return obj.get() == cooler.get(); }),
                  coolers.end());
}

void Room::notifyCoolers(float& currentTemperature) {
    for (const auto& cooler: coolers)
        cooler->cool(currentTemperature);
}

void Room::applyThermalExchange() {
    currentTemperature += thermalExchangeValues[static_cast<size_t>(thermalExchange)];
    currentTemperature = roundTemperature(currentTemperature);
}


float Room::roundTemperature(float temperature) {
    return static_cast<float>(round(temperature * 10)) / 10;
}

string Room::temperatureToString(float temperature) {
    string temperatureString = to_string(temperature);
    return temperatureString.substr(0, temperatureString.find('.') + 2);
}