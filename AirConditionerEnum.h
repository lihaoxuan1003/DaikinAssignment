//
// Created by HLi on 10/04/2023.
//

#ifndef DAIKINASSIGNMENT_AIRCONDITIONERENUM_H
#define DAIKINASSIGNMENT_AIRCONDITIONERENUM_H

#include "Room.h"
#include <array>

enum class AirConditionerPowerSettings {
    Off = 0,
    Eco,
    Low,
    Medium,
    High,
    NumberOfSettings
};

// the increments of current temperature in 10 mins
constexpr array<float, static_cast<size_t>(AirConditionerPowerSettings::NumberOfSettings)> temperatureIncrements = {
        0.0,
        0.1,
        0.2,
        0.3,
        0.4
};

// the decrements of current temperature in 10 mins
constexpr array<float, static_cast<size_t>(AirConditionerPowerSettings::NumberOfSettings)> temperatureDecrements = {
         0.0,
        -0.1,
        -0.2,
        -0.3,
        -0.4
};

#endif //DAIKINASSIGNMENT_AIRCONDITIONERENUM_H
