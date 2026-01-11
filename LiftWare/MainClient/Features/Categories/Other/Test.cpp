#include "Test.h"
#include "../../../Lift.h"

Test::Test() : Feature("Test", "Test feature.", Category::OTHER) {
};

void Test::onEnable() {
	Lift::displayMessage("on enable working uwu");
}

void Test::onDisable() {
	Lift::displayMessage("on disable working uwu");
}

void Test::onNormalTick(LocalPlayer* localPlayer) {
	if (!localPlayer) {
		Lift::displayMessage("local player being naughty grr");
		return;
	}
	Lift::displayMessage("onnormaltick working uwu");
}