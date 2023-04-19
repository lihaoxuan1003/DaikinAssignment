//
// Created by HLi on 09/04/2023.
//

#ifndef DAIKINASSIGNMENT_AIRCONDITIONER_H
#define DAIKINASSIGNMENT_AIRCONDITIONER_H

#include "../Heater.h"
#include "../Cooler.h"
#include "AirConditionerEnum.h"

class AirConditioner : public Heater, public Cooler{
public:
    explicit AirConditioner(AirConditionerPowerSettings powerSetting);
    AirConditioner() = delete;

    ~AirConditioner() override = default;

    void cool(float& temperature) override;
    void heat(float& temperature) override;

    void setCoolerSetting(int setting) override;
    void setHeaterSetting(int setting) override;
    int getCoolerSetting() override;
    int getHeaterSetting() override;
    int getNumberOfCoolerSettings() override;
    int getNumberOfHeaterSettings() override;

private:
    AirConditionerPowerSettings powerSetting;
};


#endif //DAIKINASSIGNMENT_AIRCONDITIONER_H
