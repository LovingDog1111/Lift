#include "Test.h"
#include "../../../Lift.h"

Test::Test() : Feature("Test", "Test feature.", Category::OTHER) {
};

void Test::onEnable() {
	Lift::displayMessage("on enable");
}

void Test::onDisable() {
	Lift::displayMessage("on disable");
}

void Test::onNormalTick(LocalPlayer* localPlayer) {
	if (!localPlayer) {
		Lift::displayMessage("local player being naughty grr");
		return;
	}
	localPlayer->resetRot();
}

void Test::onD2DRender() {
	D2D::drawText(Vector2<float>(0.f, 0.f), "MEOW MEOW MEOW TEST", Color(255, 0, 0), 2.0f);
}