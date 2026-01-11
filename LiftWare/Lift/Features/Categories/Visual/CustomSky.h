#pragma once
#include "../../FeatureBase/FeatureBase.h"

class CustomSky : public Feature {
private:
public:
    CustomSky();

    float intensity = 0.01f;
    bool custom = false;
    bool night = true;
};
