#pragma once
#include "../../FeatureBase/FeatureBase.h"

class ArrayList : public Feature {
public:
    ArrayList();
    void onD2DRender() override;
};
