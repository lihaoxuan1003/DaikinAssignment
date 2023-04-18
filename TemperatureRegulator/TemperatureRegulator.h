//
// Created by haoxuan.li on 4/14/2023.
//

#ifndef DAIKINASSIGNMENT_TEMPERATUREREGULATOR_H
#define DAIKINASSIGNMENT_TEMPERATUREREGULATOR_H

#include <limits>
#include <queue>
#include "../Room/Room.h"
#include "TemperatureRegulatorEnum.h"

// Based on the heater at my home and the one in my office
constexpr float MIN_VALID_ADJUSTABLE_TEMPERATURE = 5;
constexpr float MAX_VALID_ADJUSTABLE_TEMPERATURE = 30;

class TemperatureRegulator {
public:

    TemperatureRegulator(shared_ptr<Room> room, float minTemperature, float maxTemperature);

    virtual ~TemperatureRegulator() = default;

    void temperatureRegulationOn();
    void temperatureRegulationOff();
    void regulateTemperature();


    float getMinTemperature() const;
    float getMaxTemperature() const;
    const shared_ptr<Room>& getRoom() const;
    TemperatureRegulationModes getRegulationMode() const;

    void setMinTemperature(float minTemperature);
    void setMaxTemperature(float maxTemperature);
    void setRoom(const shared_ptr<Room>& room);
    void setRegulationMode(TemperatureRegulationModes regulationMode);

private:
    shared_ptr<Room> room;
    float minTemperature = numeric_limits<float>::max();
    float maxTemperature = numeric_limits<float>::min();
    float previousTemperature;
    bool temperatureRegulationEnabled = false;
    bool autoHeatingSufficient = false;
    bool autoCoolingSufficient = false;
    TemperatureRegulationModes regulationMode = TemperatureRegulationModes::UserControl;

    queue<int> heatersSettingsBackup;
    queue<int> coolersSettingsBackup;

    void backupUserSettings();
    void restoreUserSettings();
    void autoAdjustHeaters();
    void autoAdjustCoolers();
    void turboAdjustHeaters();
    void turboAdjustCoolers();
    bool isHeatingPowerAtMaxLevel();
    bool isCoolingPowerAtMaxLevel();
    bool isHeatingSufficient();
    bool isCoolingSufficient();
    void isAdjustableTemperatureValid(float temperature);
    float roundTemperature(float temperature);
    string temperatureToString(float temperature);
};


#endif //DAIKINASSIGNMENT_TEMPERATUREREGULATOR_H
