#pragma once
#include <string>
#include <vector>
#include "../../../Libs/json.hpp"
#include "../../../SDK/Game.h"

#include "Settings/Setting.h"
#include "Settings/BoolSetting.h"
#include "Settings/KeybindSetting.h"

enum class Category {
	COMBAT = 0,
	EXPLOIT = 1,
	OTHER = 2,
	PLAYER = 3,
	VISUAL = 4
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
protected:
	inline Setting* registerSetting(Setting* setting) {
		this->settings.push_back(setting);
		return setting;
	}
public:
	Feature(std::string moduleName, std::string des, Category c, int k = 0x0);
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
	virtual bool isVisible();
	virtual bool isHoldMode();
	virtual int getKeybind();
	virtual void setKeybind(int key);
	virtual bool runOnBackground();
	virtual void setEnabled(bool enable);
	virtual void toggle();
	virtual void onDisable();
	virtual void onEnable();
	virtual void onKeyUpdate(int key, bool isDown);
	virtual void onNormalTick(LocalPlayer* localPlayer);
	virtual void onLoadConfig(void* conf);
	virtual void onSaveConfig(void* conf);
};
