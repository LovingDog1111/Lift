#include "FeatureFactory.h"
#include "../Lift.h"

void FeatureFactory::init() {
	moduleList.push_back(new Test());
	moduleList.push_back(new ArrayList());

	std::sort(moduleList.begin(), moduleList.end(), [](Feature* lhs, Feature* rhs) {
		return lhs->getModuleName() < rhs->getModuleName();
		});

	Logger::Log("Feature Factory Initialized");
}

void FeatureFactory::shutdown() {
	for (auto& mod : moduleList) {
		mod->setEnabled(false);
	}
	for (auto& mod : moduleList) {
		delete mod;
		mod = nullptr;
	}
	moduleList.clear();
}

void FeatureFactory::onKeyUpdate(int key, bool isDown) {
	if (!Lift::isInitialized())
		return;

	for (auto& mod : moduleList) {
		mod->onKeyUpdate(key, isDown);
	}
}

void FeatureFactory::onNormalTick(LocalPlayer* localPlayer) {
	if (!Lift::isInitialized())
		return;

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			mod->onNormalTick(localPlayer);
		}
	}
}

void FeatureFactory::onD2DRender() {
	if (!Lift::isInitialized())
		return;

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			mod->onD2DRender();
		}
	}
}

void FeatureFactory::onUpdateRotation(LocalPlayer* localPlayer) {
	if (!Lift::isInitialized())
		return;

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->runOnBackground()) {
			mod->onUpdateRotation(localPlayer);
		}
	}
}

void FeatureFactory::onLoadConfig(void* conf) {
	if (!Lift::isInitialized())
		return;

	for (auto& mod : moduleList) {
		mod->onLoadConfig(conf);
	}
}

void FeatureFactory::onSaveConfig(void* conf) {
	if (!Lift::isInitialized())
		return;

	for (auto& mod : moduleList) {
		mod->onSaveConfig(conf);
	}
}