//
// Created by HLi on 09/04/2023.
//

#ifndef DAIKINASSIGNMENT_RADIATOR_H
#define DAIKINASSIGNMENT_RADIATOR_H

#include "Heater.h"
#include "RadiatorEnum.h"

#define RADIATOR_TEMPERATURE_INCREMENT 0.1

class Radiator : public Heater {
public:
    explicit Radiator(RadiatorValveSettings valveSetting);
    Radiator() = delete;

    ~Radiator() override = default;;

    void heat(float &temperature) override;

    RadiatorValveSettings getValveSetting() const;

    void setValveSetting(RadiatorValveSettings valveSetting);

private:
    RadiatorValveSettings valveSetting;
};


#endif //DAIKINASSIGNMENT_RADIATOR_H
