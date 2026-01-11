#pragma once
#include "FeatureBase/FeatureBase.h"
#include "Categories/Other/Test.h"
#include "Categories/Visual/ArrayList.h"

class FeatureFactory {
public:
	static inline std::vector<Feature*> moduleList;

	static void init();
	static void shutdown();

	template <typename TRet>
	static TRet* getFeature() {
		for (Feature* mod : moduleList) {
			TRet* result = dynamic_cast<TRet*>(mod);
			if (result == nullptr)
				continue;
			return result;
		}
		return nullptr;
	}

	static void onKeyUpdate(int key, bool isDown);
	static void onNormalTick(LocalPlayer* localPlayer);
	static void onUpdateRotation(LocalPlayer* localPlayer);
	static void onLoadConfig(void* conf);
	static void onSaveConfig(void* conf);
	static void onD2DRender();
};
