//
// Created by HLi on 10/04/2023.
//

#ifndef DAIKINASSIGNMENT_RADIATORENUM_H
#define DAIKINASSIGNMENT_RADIATORENUM_H

#include <array>
#include "../Room/Room.h"
#include "../TemperatureRegulator/TemperatureRegulator.h"

enum class RadiatorValveSettings {
    SettingZero = 0,
    SettingOne,
    SettingTwo,
    SettingThree,
    SettingFour,
    SettingFive,
    NumberOfSettings
};

constexpr array<float, static_cast<size_t>(RadiatorValveSettings::NumberOfSettings)> radiatorValveTemperatures = {
        MIN_ROOM_TEMPERATURE,
        MIN_VALID_ADJUSTABLE_TEMPERATURE,
        MIN_VALID_ADJUSTABLE_TEMPERATURE + 5.0,
        MIN_VALID_ADJUSTABLE_TEMPERATURE + 10.0,
        MIN_VALID_ADJUSTABLE_TEMPERATURE + 20.0,
        MIN_VALID_ADJUSTABLE_TEMPERATURE + 30.0
};

#endif //DAIKINASSIGNMENT_RADIATORENUM_H
