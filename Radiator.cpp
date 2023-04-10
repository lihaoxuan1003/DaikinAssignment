//
// Created by HLi on 09/04/2023.
//

#include "Radiator.h"


void Radiator::heat(float &temperature) {
    if (temperature < radiatorValveTemperatures[static_cast<size_t>(valveSetting)])
        temperature += RADIATOR_TEMPERATURE_INCREMENT;
}

Radiator::Radiator(RadiatorValveSettings valveSetting) : valveSetting(valveSetting) {}

RadiatorValveSettings Radiator::getValveSetting() const {
    return valveSetting;
}

void Radiator::setValveSetting(RadiatorValveSettings valveSetting) {
    this->valveSetting = valveSetting;
}


