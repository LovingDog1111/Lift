#pragma once
#include "../FuncHook/FuncHook.h"

class BrightnessFogColorHook : public FuncHook {
private:
    using func_t = float* (__fastcall*)(float*, void*, void*, void*);
    static inline func_t oFunc;

    static float* HookCallback(float* a1, void* a2, void* a3, void* a4) {
        if (!FeatureFactory::init)
            return oFunc(a1, a2, a3, a4);

        static CustomSky* CustomSkyMod = FeatureFactory::getFeature<CustomSky>();

        if (CustomSkyMod->isEnabled() && CustomSkyMod->night) {
            static float nightColor[4] = { 0.f, 0.f, 0.2f, 1.f };
            return nightColor;
        }

        static float finalColor[4];

        if (CustomSkyMod->isEnabled()) {
            if (CustomSkyMod->custom) {
                static float h = 0.0f, s = 0.8f, v = 1.0f;

                Colors::ColorConvertHSVtoRGB(h, s, v, finalColor[0], finalColor[1],
                    finalColor[2]);
                finalColor[3] = 1.0f;

                h += CustomSkyMod->intensity;
                if (h >= 1.0f)
                    h = 0.0f;

                return finalColor;
            }
            else {
                Color accentColor1 = Colors::getWaveColor(FeatureFactory::getFeature<Theme>()->color1, FeatureFactory::getFeature<Theme>()->color2, 0);

                finalColor[0] = (accentColor1.r / 255.f);
                finalColor[1] = (accentColor1.g / 255.f);
                finalColor[2] = (accentColor1.b / 255.f);
                finalColor[3] = 1.0f;

                return finalColor;
            }
        }

        return oFunc(a1, a2, a3, a4);
    }

public:
    BrightnessFogColorHook() {
        OriginFunc = (void**)&oFunc;
        func = (void*)&HookCallback;
    }
};