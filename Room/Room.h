//
// Created by haoxuan.li on 4/4/2023.
//

#ifndef DAIKINASSIGNMENT_ROOM_H
#define DAIKINASSIGNMENT_ROOM_H

#include <string>
#include <memory>
#include <vector>
#include "../Heater.h"
#include "../Cooler.h"
#include "ThermalExchangeEnum.h"

using namespace std;

// Based on the yearly currentTemperature in Belgium
constexpr float MIN_ROOM_TEMPERATURE = -10;
constexpr float MAX_ROOM_TEMPERATURE = 40;

class Room {
public:
    explicit Room(float currentTemperature);

    virtual ~Room() = default;

    void applyThermalExchange();

    void addHeater(shared_ptr<Heater> heater);
    void removeHeater(shared_ptr<Heater> heater);
    void notifyHeaters(float& currentTemperature);

    void addCooler(shared_ptr<Cooler> cooler);
    void removeCooler(shared_ptr<Cooler> cooler);
    void notifyCoolers(float& currentTemperature);

    const vector<shared_ptr<Heater>>& getHeaters() const;
    const vector<shared_ptr<Cooler>>& getCoolers() const;
    float& getCurrentTemperature();
    ThermalExchange getThermalExchange() const;
    void setCurrentTemperature(float temperature);
    void setThermalExchange(ThermalExchange thermalExchange);

private:
    float currentTemperature;
    ThermalExchange thermalExchange = ThermalExchange::None;
    vector<shared_ptr<Heater>> heaters;
    vector<shared_ptr<Cooler>> coolers;

    float roundTemperature(float temperature);
    string temperatureToString(float temperature);
};


#endif //DAIKINASSIGNMENT_ROOM_H
