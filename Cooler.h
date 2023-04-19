//
// Created by HLi on 09/04/2023.
//

#ifndef DAIKINASSIGNMENT_COOLER_H
#define DAIKINASSIGNMENT_COOLER_H

class Cooler {
public:
    virtual ~Cooler() = default;
    virtual void cool(float& temperature) = 0;;
    virtual void setCoolerSetting(int setting) = 0;
    virtual int getCoolerSetting() = 0;
    virtual int getNumberOfCoolerSettings() = 0;

};

#endif //DAIKINASSIGNMENT_COOLER_H
