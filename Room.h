//
// Created by haoxuan.li on 4/4/2023.
//

#ifndef DAIKINASSIGNMENT_ROOM_H
#define DAIKINASSIGNMENT_ROOM_H

#include <string>
#include <memory>
#include "Heater.h"
#include "Cooler.h"

using namespace std;

// Based on the yearly temperature in Belgium
constexpr float MIN_ROOM_TEMPERATURE = -10;
constexpr float MAX_ROOM_TEMPERATURE = 40;
// Based on the heater at my home and the one in my office
constexpr float MIN_VALID_ADJUSTABLE_TEMPERATURE = 5;
constexpr float MAX_VALID_ADJUSTABLE_TEMPERATURE = 30;

class Room {
public:
    Room(float temperature, float minTemperature, float maxTemperature, Heater* heater, Cooler* cooler);

    virtual ~Room() = default;

    void temperatureRegulationOn();
    void temperatureRegulationOff();

    float getTemperature() const;
    float getMinTemperature() const;
    float getMaxTemperature() const;
    void setTemperature(float temperature);
    void setMinTemperature(float minTemperature);
    void setMaxTemperature(float maxTemperature);

private:
    float temperature;
    float minTemperature;
    float maxTemperature;
    bool temperatureRegulationEnabled = false;
    unique_ptr<Heater> heater;
    unique_ptr<Cooler> cooler;
    void regulateTemperature();

    void isAdjustableTemperatureValid(float temperature);
    void roundTemperature(float &temperature);
    string temperatureToString(float temperature);
};


#endif //DAIKINASSIGNMENT_ROOM_H
