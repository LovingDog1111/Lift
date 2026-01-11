#include "CustomSky.h"

CustomSky::CustomSky() : Feature("CustomSky", "CustomSky.", Category::VISUAL) {
    registerSetting(new SliderSetting<float>("Intensity", "NULL", &intensity, 0.05f, 0.01f, 0.1f));
    registerSetting(new BoolSetting("Night", "NULL", &night, false));
    registerSetting(new BoolSetting("Custom", "NULL", &custom, false));
}