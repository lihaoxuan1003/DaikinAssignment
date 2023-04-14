//
// Created by haoxuan.li on 4/14/2023.
//

#include <cmath>
#include <utility>
#include "TemperatureRegulator.h"

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

    if(autoControlEnabled)
    {

    }

    auto& currentTemperature = room->getCurrentTemperature();

    if (minTemperature - currentTemperature >= FLOATING_OPERATION_PRECISION)
        room->notifyHeaters(currentTemperature);


    if (currentTemperature - maxTemperature >= FLOATING_OPERATION_PRECISION)
        room->notifyCoolers(currentTemperature);

    currentTemperature = roundTemperature(currentTemperature);
    //   cout << "Temperature regulated current room currentTemperature is: " << temperatureToString(currentTemperature) << endl;


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

const shared_ptr<Room> &TemperatureRegulator::getRoom() const {
    return room;
}

void TemperatureRegulator::setRoom(const shared_ptr<Room>& room) {
    this->room = room;
}

void TemperatureRegulator::autoControlOn() {
    autoControlEnabled = true;
}

void TemperatureRegulator::autoControlOff() {
    autoControlEnabled = false;
}
