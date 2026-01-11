#pragma once
#include "../../FeatureBase/FeatureBase.h"

class NoEatBob : public Feature {
public:
    NoEatBob();
    ~NoEatBob();

    void onEnable() override;
    void onDisable() override;

private:
    void tryLocate();
    void applyPatch();
    void restorePatch();

    inline static void* sAddr = nullptr;
    inline static bool sSaved = false;
    inline static bool sPatched = false;
    uint8_t sOg5[5]{};
};
