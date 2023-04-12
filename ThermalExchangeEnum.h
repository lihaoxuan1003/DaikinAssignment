//
// Created by haoxuan.li on 4/11/2023.
//

// The enumeration used to represent possible thermal exchange values is not a realistic model of real-world scenarios.
// It is used only for testing purposes.

#ifndef DAIKINASSIGNMENT_THERMALEXCHANGEENUM_H
#define DAIKINASSIGNMENT_THERMALEXCHANGEENUM_H

#include <array>

using namespace std;

enum class ThermalExchange {
    NegativeVeryHigh = 0,
    NegativeHigh,
    NegativeMedium,
    NegativeLow,
    None,
    PositiveLow,
    PositiveMedium,
    PositiveHigh,
    PositiveVeryHigh,
    NumberOfThermalExchangeLevels,
};


// the decrements of current temperature in 10 mins
constexpr array<float, static_cast<size_t>(ThermalExchange::NumberOfThermalExchangeLevels)> thermalExchangeValues = {
        -0.8,
        -0.4,
        -0.2,
        -0.1,
        0.0,
        0.1,
        0.2,
        0.4,
        0.8
};

#endif //DAIKINASSIGNMENT_THERMALEXCHANGEENUM_H
