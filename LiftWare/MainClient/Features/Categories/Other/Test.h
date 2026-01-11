#pragma once
#include "../../FeatureBase/FeatureBase.h"

class Test : public Feature {
public:
	Test();
	void onNormalTick(LocalPlayer* localPlayer) override;
	void onEnable() override;
	void onDisable() override;
};