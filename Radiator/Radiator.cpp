//
// Created by HLi on 09/04/2023.
//

#include "Radiator.h"
#include <iostream>

void Radiator::heat(float& temperature) {
    if (temperature < radiatorValveTemperatures[static_cast<size_t>(valveSetting)])
        temperature += RADIATOR_TEMPERATURE_INCREMENT;
}

Radiator::Radiator(RadiatorValveSettings valveSetting) : valveSetting(valveSetting) {}


void Radiator::setHeaterSetting(int setting) {
    if (setting < static_cast<int>(RadiatorValveSettings::SettingZero) || setting >= static_cast<int>(RadiatorValveSettings::NumberOfSettings))
        throw invalid_argument("Invalid radiator valve setting.");
    valveSetting = static_cast<RadiatorValveSettings>(setting);
}

int Radiator::getNumberOfHeaterSettings() {
    return static_cast<int>(RadiatorValveSettings::NumberOfSettings);
}

int Radiator::getHeaterSetting() {
    return static_cast<int>(valveSetting);
}
