//
// Created by haoxuan.li on 4/18/2023.
//
#include "TemperatureRegulatorEnum.h"

string regulationModeToString(TemperatureRegulationModes regulationMode) {
    switch (regulationMode) {
        case TemperatureRegulationModes::UserControl:
            return "UserControl";
        case TemperatureRegulationModes::AutoControl:
            return "AutoControl";
        case TemperatureRegulationModes::Turbo:
            return "Turbo";
        default:
            return "unknown";
    }
}