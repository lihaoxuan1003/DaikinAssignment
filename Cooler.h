//
// Created by HLi on 09/04/2023.
//

#ifndef DAIKINASSIGNMENT_COOLER_H
#define DAIKINASSIGNMENT_COOLER_H

class Cooler {
public:
    virtual ~Cooler() = default;
    virtual void cool(float& temperature) = 0;
    virtual void update(float& currentTemperature, float minTemperature, float maxTemperature) = 0;
};

#endif //DAIKINASSIGNMENT_COOLER_H
