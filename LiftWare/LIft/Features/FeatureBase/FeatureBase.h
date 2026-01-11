#pragma once
#include <string>
#include <vector>
#include "../../../Libs/json.hpp"
#include "../../../SDK/Game.h"

#include "Settings/Setting.h"
#include "Settings/Bool.h"
#include "Settings/Keybind.h"
#include "Settings/Slider.h"

#include "../../Renderer/D2D.h"

enum class Category {
	COMBAT = 0,
	EXPLOIT = 1,
	OTHER = 2,
	PLAYER = 3,
	VISUAL = 4,
	MOVEMENT = 5
};

class Feature {
private:
	std::string name;
	std::string description;
	Category category;
	bool enabled = false;
	bool visible = true;
	int keybind = 0x0;
	int toggleMode = 0;

	std::vector<Setting*> settings;
public:
	bool extended = false;
	float animProgress = 0.0f;
	float enableAnimProgress = 0.f;
	float disableAnimProgress = 0.f;
protected:
	inline Setting* registerSetting(Setting* setting) {
		this->settings.push_back(setting);
		return setting;
	}
public:
	Feature(std::string featureName, std::string des, Category c, int k = 0x0);
	~Feature();

	inline std::string getModuleName() {
		return this->name;
	}

	inline std::string getDescription() {
		return this->description;
	}

	inline Category getCategory() {
		return this->category;
	}

	inline std::vector<Setting*>& getSettingList() {
		return this->settings;
	}
public:
	virtual std::string getModeText();
	virtual bool isEnabled();
	virtual void onMouseUpdate(Vector2<float> mousePos, char mouseButton, char isDown);
	virtual bool isVisible();
	virtual bool isHoldMode();
	virtual int getKeybind();
	virtual void setKeybind(int key);
	virtual bool runOnBackground();
	virtual void setEnabled(bool enable);
	virtual void toggle();
	virtual void onDisable();
	virtual void onEnable();
	virtual void onD2DRender();
	virtual void onKeyUpdate(int key, bool isDown);
	virtual void onNormalTick(LocalPlayer* localPlayer);
	virtual void onUpdateRotation(LocalPlayer* localPlayer);
	virtual void onLoadConfig(void* conf);
	virtual void onSaveConfig(void* conf);
};
