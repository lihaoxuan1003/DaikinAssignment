//
// Created by HLi on 17/04/2023.
//

#ifndef DAIKINASSIGNMENT_TEMPERATUREREGULATORENUM_H
#define DAIKINASSIGNMENT_TEMPERATUREREGULATORENUM_H

#include <string>

using namespace std;

enum class TemperatureRegulationModes {
    UserControl = 0,
    AutoControl,  // Increase the power of heaters/coolers gradually to reach the target temperature economically
    Turbo,        // Set the power of heaters/coolers to maximum to quickly reach the target temperature
    NumberOfModes
};

string regulationModeToString(TemperatureRegulationModes regulationMode);

#endif //DAIKINASSIGNMENT_TEMPERATUREREGULATORENUM_H
