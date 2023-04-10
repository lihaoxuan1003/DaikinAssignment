//
// Created by HLi on 10/04/2023.
//

#ifndef DAIKINASSIGNMENT_AIRCONDITIONERENUM_H
#define DAIKINASSIGNMENT_AIRCONDITIONERENUM_H

#include "Room.h"
#include <array>

enum class AirConditionerPowerSettings {
    Eco = 0,
    Low,
    Medium,
    High,
    NumberOfSettings
};

constexpr array<float, static_cast<size_t>(AirConditionerPowerSettings::NumberOfSettings)> temperatureIncrements = {
        0.1,
        0.2,
        0.3,
        0.4
};

constexpr array<float, static_cast<size_t>(AirConditionerPowerSettings::NumberOfSettings)> temperatureDecrements = {
        -0.1,
        -0.2,
        -0.4,
        -0.8
};

#endif //DAIKINASSIGNMENT_AIRCONDITIONERENUM_H
