//
// Created by HLi on 09/04/2023.
//

#include "AirConditioner.h"

void AirConditioner::heat(float& temperature) {
    temperature += temperatureIncrements[static_cast<size_t>(powerSetting)];
}

void AirConditioner::cool(float& temperature) {
    temperature += temperatureDecrements[static_cast<size_t>(powerSetting)];
}

AirConditioner::AirConditioner(AirConditionerPowerSettings powerSetting) : powerSetting(powerSetting) {}

void
AirConditioner::update(float& currentTemperature, float minTemperature, float maxTemperature) {
    if(currentTemperature < minTemperature)
        heat(currentTemperature);

    if (currentTemperature > maxTemperature)
        cool(currentTemperature);
}

AirConditionerPowerSettings AirConditioner::getPowerSetting() const {
    return powerSetting;
}

void AirConditioner::setPowerSetting(AirConditionerPowerSettings powerSetting) {
    this->powerSetting = powerSetting;
}
