//
// Created by haoxuan.li on 4/4/2023.
//

#ifndef DAIKINASSIGNMENT_ROOM_H
#define DAIKINASSIGNMENT_ROOM_H

#include <string>
#include <memory>
#include <vector>
#include <limits>
#include "Heater.h"
#include "Cooler.h"
#include "ThermalExchangeEnum.h"

using namespace std;

// Based on the yearly currentTemperature in Belgium
constexpr float MIN_ROOM_TEMPERATURE = -10;
constexpr float MAX_ROOM_TEMPERATURE = 40;
// Based on the heater at my home and the one in my office
constexpr float MIN_VALID_ADJUSTABLE_TEMPERATURE = 5;
constexpr float MAX_VALID_ADJUSTABLE_TEMPERATURE = 30;

class Room {
public:
    Room(float currentTemperature, float minTemperature, float maxTemperature);

    virtual ~Room() = default;

    void temperatureRegulationOn();
    void temperatureRegulationOff();
    void regulateTemperature();
    void applyThermalExchange();

    void addHeater(shared_ptr<Heater> heater);
    void removeHeater(shared_ptr<Heater> heater);
    void notifyHeaters(float& currentTemperature, float minTemperature, float maxTemperature);

    void addCooler(shared_ptr<Cooler> cooler);
    void removeCooler(shared_ptr<Cooler> cooler);
    void notifyCoolers(float& currentTemperature, float minTemperature, float maxTemperature);

    float getTemperature() const;
    float getMinTemperature() const;
    float getMaxTemperature() const;
    void setTemperature(float temperature);
    void setMinTemperature(float minTemperature);
    void setMaxTemperature(float maxTemperature);
    void setThermalExchange(ThermalExchange thermalExchange);

private:
    float currentTemperature;
    float minTemperature = numeric_limits<float>::max();
    float maxTemperature = numeric_limits<float>::min();
    ThermalExchange thermalExchange = ThermalExchange::None;
    bool temperatureRegulationEnabled = false;
    vector<shared_ptr<Heater>> heaters;
    vector<shared_ptr<Cooler>> coolers;

    void isAdjustableTemperatureValid(float temperature);
    void roundTemperature(float& temperature);
    string temperatureToString(float temperature);

};


#endif //DAIKINASSIGNMENT_ROOM_H
