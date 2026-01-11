#pragma once
#include "../../Math/IncludeAll.h"
#include "../../Values/Pads.h"

class GuiData {
private:
	char pad_0x0[Pads::GuiData_pad_0x0];
public:
	Vector2<float> windowSizeReal;
	Vector2<float> windowSizeReal2;
	Vector2<float> windowSizeScaled;
	double guiScale1;
	float guiScale2;
}; 