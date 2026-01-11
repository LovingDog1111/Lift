#pragma once
#include "../../FeatureBase/FeatureBase.h"

class Theme : public Feature {
public:
    Theme();

    void onD2DRender() override;
    bool isEnabled() override;
    bool isVisible() override;

    int selectedTheme = 0;
    Color color1;
    Color color2;
private:
    void updateColors();
};
