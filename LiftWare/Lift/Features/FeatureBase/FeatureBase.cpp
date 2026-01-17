#include "FeatureBase.h"

using json = nlohmann::json;

Feature::Feature(std::string featureName, std::string des, Category c, int k) {
	this->name = featureName;
	this->description = des;
	this->category = c;
	this->keybind = k;

	registerSetting(new BoolSetting("Hide", "Visible on arraylist", &visible, true));
	registerSetting(new KeybindSetting("Keybind", "Keybind of module", &keybind, k));
}

Feature::~Feature() {
	for (auto& setting : settings) {
		delete setting;
		setting = nullptr;
	}
	settings.clear();
}

std::string Feature::getModeText() {
	return "NULL";
}

void Feature::onKeyUpdate(int key, bool isDown) {
	if (getKeybind() == key) {
		if (isHoldMode()) {
			setEnabled(isDown);
		}
		else {
			if (isDown) {
				toggle();
			}
		}
	}
}

void Feature::onMouseUpdate(Vector2<float> mousePos, char mouseButton, char isDown) {}

bool Feature::isEnabled() {
	return enabled;
}

bool Feature::isVisible() {
	return visible;
}

bool Feature::isHoldMode() {
	return toggleMode;
}

int Feature::getKeybind() {
	return keybind;
}

void Feature::setKeybind(int key) {
	this->keybind = key;
}

bool Feature::runOnBackground() {
	return false;
}

void Feature::setEnabled(bool enable) {
	if (this->enabled != enable) {
		this->enabled = enable;

		if (enable) {
			this->onEnable();
		}
		else {
			this->onDisable();
		}
	}
}

void Feature::toggle() {
	setEnabled(!enabled);
}

void Feature::onDisable() {
	this->enableAnimProgress = 0.0f;
	this->disableAnimProgress = 1.0f;
}

void Feature::onEnable() {
	this->enableAnimProgress = 1.0f;
	this->disableAnimProgress = 0.0f;
}

void Feature::onNormalTick(LocalPlayer* localPlayer) {
}

void Feature::onD2DRender() {
}

void Feature::onUpdateRotation(LocalPlayer* localPlayer) {
}

void Feature::onLoadConfig(void* confVoid) {
	json* conf = reinterpret_cast<json*>(confVoid);
	std::string modName = this->getModuleName();

	if (conf->contains(modName)) {
		json obj = conf->at(modName);
		if (obj.is_null())
			return;

		if (obj.contains("enabled")) {
			this->setEnabled(obj.at("enabled").get<bool>());
		}

		for (auto& setting : settings) {
			std::string settingName = setting->name;

			if (obj.contains(settingName)) {
				json confValue = obj.at(settingName);
				if (confValue.is_null())
					continue;

				switch (setting->type) {
				case SettingType::BOOL: {
					BoolSetting* boolSetting = static_cast<BoolSetting*>(setting);
					(*boolSetting->value) = confValue.get<bool>();
					break;
				}
				case SettingType::KEYBIND: {
					KeybindSetting* keybindSetting = static_cast<KeybindSetting*>(setting);
					(*keybindSetting->value) = confValue.get<int>();
					break;
				}
				}
			}
		}
	}
}

void Feature::onSaveConfig(void* confVoid) {
	json* conf = reinterpret_cast<json*>(confVoid);
	std::string modName = this->getModuleName();
	json obj = (*conf)[modName];

	obj["enabled"] = this->isEnabled();

	for (auto& setting : settings) {
		std::string settingName = setting->name;

		switch (setting->type) {
		case SettingType::BOOL: {
			BoolSetting* boolSetting = static_cast<BoolSetting*>(setting);
			obj[settingName] = (*boolSetting->value);
			break;
		}
		case SettingType::KEYBIND: {
			KeybindSetting* keybindSetting = static_cast<KeybindSetting*>(setting);
			obj[settingName] = (*keybindSetting->value);
			break;
		}
		}
	}

	(*conf)[modName] = obj;
}