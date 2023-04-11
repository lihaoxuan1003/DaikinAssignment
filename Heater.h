//
// Created by HLi on 09/04/2023.
//

#ifndef DAIKINASSIGNMENT_HEATER_H
#define DAIKINASSIGNMENT_HEATER_H

class Heater {
public:
    virtual ~Heater() = default;
    virtual void heat(float& temperature) = 0;
    virtual void update(float& currentTemperature, float minTemperature, float maxTemperature) = 0;
};


#endif //DAIKINASSIGNMENT_HEATER_H
