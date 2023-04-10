//
// Created by HLi on 09/04/2023.
//

#include "AirConditioner.h"

void AirConditioner::heat(float &temperature) {
    temperature += temperatureIncrements[static_cast<size_t>(powerSetting)];
}

void AirConditioner::cool(float &temperature) {
    temperature += temperatureDecrements[static_cast<size_t>(powerSetting)];;
}

AirConditioner::AirConditioner(AirConditionerPowerSettings powerSetting) : powerSetting(powerSetting) {}
