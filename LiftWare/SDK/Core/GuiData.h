#pragma once
#include "../../Math/IncludeAll.h"

class GuiData {
private:
	char pad_0x0[0x30];
public:
	Vector2<float> windowSizeReal;
	Vector2<float> windowSizeReal2;
	Vector2<float> windowSizeScaled;
	double guiScale1;
	float guiScale2;
}; 