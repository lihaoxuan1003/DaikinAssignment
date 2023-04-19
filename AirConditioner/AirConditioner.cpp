//
// Created by HLi on 09/04/2023.
//

#include "AirConditioner.h"

AirConditioner::AirConditioner(AirConditionerPowerSettings powerSetting) : powerSetting(powerSetting) {}

void AirConditioner::heat(float& temperature) {
    temperature += temperatureIncrements[static_cast<size_t>(powerSetting)];
}

void AirConditioner::cool(float& temperature) {
    temperature += temperatureDecrements[static_cast<size_t>(powerSetting)];
}

void AirConditioner::setCoolerSetting(int setting) {
    powerSetting = static_cast<AirConditionerPowerSettings>(setting);
}

int AirConditioner::getCoolerSetting() {
    return static_cast<int>(powerSetting);
}

int AirConditioner::getNumberOfCoolerSettings() {
    return static_cast<int>(AirConditionerPowerSettings::NumberOfSettings);
}

void AirConditioner::setHeaterSetting(int setting) {
    powerSetting = static_cast<AirConditionerPowerSettings>(setting);
}

int AirConditioner::getHeaterSetting() {
    return static_cast<int>(powerSetting);
}

int AirConditioner::getNumberOfHeaterSettings() {
    return static_cast<int>(AirConditionerPowerSettings::NumberOfSettings);
}
