#pragma once
#include <string>

enum class SettingType {
	BOOL,
	KEYBIND,
	SLIDER,
	UNKNOWN
};

class Setting {
public:
	std::string name;
	std::string description;
	SettingType type = SettingType::UNKNOWN;
public:

	float selectedAnim = 0.f;
};
