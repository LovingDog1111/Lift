#include "ClickGUI.h"
#include "../../FeatureFactory.h"

ClickGUI::ClickGUI() : Feature("ClickGUI", "A module to display features.", Category::VISUAL, VK_INSERT) {

}

ClickGUI::Header::Header(std::string windowName, Vector2<float> startPos, Category c) {
	this->name = windowName;
	this->category = c;
	this->pos = startPos;
	this->extended = true;

	for (auto& mod : FeatureFactory::moduleList) {
		if (mod->getCategory() == c) {
			this->moduleList.push_back(mod);
		}
	}

	std::sort(this->moduleList.begin(), this->moduleList.end(), [](Feature* lhs, Feature* rhs) {
		return lhs->getModuleName() < rhs->getModuleName();
		});
}

void ClickGUI::onDisable() {
	Game::clientInstance->grabMouse();

	isLeftClickDown = false;
	isRightClickDown = false;
	isHoldingLeftClick = false;
	isHoldingRightClick = false;

	draggingWindowPtr = nullptr;

	capturingKbSettingPtr = nullptr;
}

void ClickGUI::onEnable() {
	Game::clientInstance->releaseMouse();
}

bool ClickGUI::isVisible() {
	return false;
}

void ClickGUI::onKeyUpdate(int key, bool isDown) {
	if (!isEnabled()) {
		if (key == getKeybind() && isDown) {
			setEnabled(true);
		}
	}
	else {
		if (isDown) {
			if (key < 192) {
				if (capturingKbSettingPtr != nullptr) {
					if (key != VK_ESCAPE)
						*capturingKbSettingPtr->value = key;
					capturingKbSettingPtr = nullptr;
					return;
				}
			}
			if (key == getKeybind() || key == VK_ESCAPE) {
				setEnabled(false);
			}
		}
	}
}

void ClickGUI::onMouseUpdate(Vector2<float> mousePosA, char mouseButton, char isDown) {
	if (!Game::getClientInstance()) return;

	mousePos = mousePosA;

	switch (mouseButton) {
	case 1:
		isLeftClickDown = isDown;
		isHoldingLeftClick = isDown;
		break;
	case 2:
		isRightClickDown = isDown;
		isHoldingRightClick = isDown;
		break;
	case 4: {
		float moveVec = (isDown < 0) ? -15.f : 15.f;
		for (auto& window : windowList) {
			if (window == draggingWindowPtr)
				continue;
			window->pos.y += moveVec;
		}
		break;
	}
	}

	if (draggingWindowPtr && !isHoldingLeftClick)
		draggingWindowPtr = nullptr;

	if (capturingKbSettingPtr && isRightClickDown) {
		*capturingKbSettingPtr->value = 0;
		capturingKbSettingPtr = nullptr;
		isRightClickDown = false;
	}
}

void ClickGUI::InitClickGUI() {
	setEnabled(false);

	Vector2<float> startPos = Vector2<float>(25.f, 4.f);

	windowList.push_back(new Header("Combat", startPos, Category::COMBAT));
	startPos.x += 190.f;
	windowList.push_back(new Header("Movement", startPos, Category::MOVEMENT));
	startPos.x += 190.f;
	windowList.push_back(new Header("Visual", startPos, Category::VISUAL));
	startPos.x += 190.f;
	windowList.push_back(new Header("Player", startPos, Category::PLAYER));
	startPos.x += 190.f;
	windowList.push_back(new Header("Exploit", startPos, Category::EXPLOIT));
	startPos.x += 190.f;
	windowList.push_back(new Header("Other", startPos, Category::OTHER));
	startPos.x += 190.f;

	initialized = true;
}

void ClickGUI::onD2DRender() {
	if (!initialized)
		return;

	if (Game::canUseMoveKeys())
		Game::clientInstance->releaseMouse();

	static Vector2<float> oldMousePos = mousePos;
	mouseDelta = mousePos - oldMousePos;
	oldMousePos = mousePos;
	std::string descriptionText = "NULL";

	Vector2<float> screenSize = Game::clientInstance->guiData->windowSizeReal;

	float textSize = 0.5f;
	float textPaddingX = 5.f * textSize;
	float textHeight = D2D::getTextHeight("X", textSize * 1.65f);

	float headerTextSize = 0.65f;
	float headerTextHeight = D2D::getTextHeight("X", textSize * 1.75f);

	float rounding = 6.f;

	Color darkerGray = Color(12.f, 12.f, 12.f);
	Color darkGray = Color(18.f, 18.f, 18.f);
	Color Gray = Color(35.f, 35.f, 35.f);
	Color SolsticeGray = Color(42.f, 42.f, 42.f);
	Color SolsticeBlack = Color(0.f, 0.f, 0.f);
	Color accentColor = Color(130.f, 211.f, 195.f);

	for (auto& window : windowList) {
		if (draggingWindowPtr == window)
			window->pos = window->pos + mouseDelta;

		float desiredWidth = 155.f + textPaddingX * 2.f;
		float headerWidth = desiredWidth;

		float clampedX = window->pos.x;
		if (clampedX + headerWidth > screenSize.x)
			clampedX = screenSize.x - headerWidth;
		if (clampedX < 0.f)
			clampedX = 0.f;

		Vector4<float> hRect(
			clampedX,
			window->pos.y,
			clampedX + headerWidth,
			window->pos.y + headerTextHeight
		);

		Vector2 hText(
			hRect.x + (headerWidth - D2D::getTextWidth(window->name, headerTextSize, true, true)) * 0.5f,
			hRect.y + (headerTextHeight - D2D::getTextHeight("X", headerTextSize, true, true)) * 0.5f
		);

		if (hRect.contains(mousePos)) {
			if (isLeftClickDown) {
				draggingWindowPtr = window;
				isLeftClickDown = false;
			}
			else if (isRightClickDown) {
				window->extended = !window->extended;
				isRightClickDown = false;
			}
		}

		D2D::fillRectangle(hRect, SolsticeBlack, rounding, D2D::CornerRoundType::TopOnly);
		D2D::drawText(hText, window->name, Color(255, 255, 255), headerTextSize, true, true);

		if (!window->extended)
			continue;

		float yOffset = hRect.w;

		for (auto& mod : window->moduleList) {
			Vector4 mRect(
				hRect.x,
				yOffset,
				hRect.z,
				yOffset + textHeight
			);

			Vector2 mText(
				mRect.x + (mRect.z - mRect.x - D2D::getTextWidth(mod->getModuleName(), textSize)) * 0.5f,
				mRect.y + (textHeight - D2D::getTextHeight("X", textSize)) * 0.5f
			);

			if (mRect.contains(mousePos)) {
				descriptionText = mod->getDescription();
				if (isLeftClickDown) {
					mod->toggle();
					isLeftClickDown = false;
				}
				else if (isRightClickDown) {
					mod->extended = !mod->extended;
					isRightClickDown = false;
				}
			}

			if (&mod == &window->moduleList.back())
				D2D::fillRectangle(mRect, mod->isEnabled() ? accentColor : Gray, rounding, D2D::CornerRoundType::BottomOnly);
			else
				D2D::fillRectangle(mRect, mod->isEnabled() ? accentColor : Gray);

			D2D::drawText(mText, mod->getModuleName(),
				mod->isEnabled() ? Color(255, 255, 255) : Color(175, 175, 175),
				textSize);

			yOffset += textHeight;

			if (!mod->extended)
				continue;

			yOffset += 1.5f;

			for (auto& setting : mod->getSettingList()) {
				if (setting->type != SettingType::BOOL &&
					setting->type != SettingType::KEYBIND)
					continue;

				Vector4 sRect(
					mRect.x + 6.f,
					yOffset,
					mRect.z - 6.f,
					yOffset + textHeight
				);

				Vector2 sText(
					sRect.x + textPaddingX,
					sRect.y + (textHeight - D2D::getTextHeight("X", textSize)) * 0.5f
				);

				if (sRect.contains(mousePos))
					descriptionText = setting->description;

				if (setting->type == SettingType::BOOL) {
					auto* bs = static_cast<BoolSetting*>(setting);
					bool& value = *bs->value;

					if (sRect.contains(mousePos) && isLeftClickDown) {
						value = !value;
						isLeftClickDown = false;
					}

					D2D::drawText(sText, setting->name,
						!value ? Color(255, 255, 255) : Color(175, 175, 175),
						textSize);

					float box = textHeight * 0.75f;
					Vector4 checkbox(
						sRect.z - box - textPaddingX,
						sRect.y + (textHeight - box) * 0.5f,
						sRect.z - textPaddingX,
						sRect.y + (textHeight - box) * 0.5f + box
					);

					if (!value)
						D2D::fillRectangle(checkbox, Color(255, 0, 0));

					D2D::drawRectangle(checkbox, Color(255, 255, 255));
				}

				if (setting->type == SettingType::KEYBIND) {
					auto* ks = static_cast<KeybindSetting*>(setting);
					int& key = *ks->value;

					if (sRect.contains(mousePos) && isLeftClickDown) {
						capturingKbSettingPtr =
							(capturingKbSettingPtr == ks ? nullptr : ks);
						isLeftClickDown = false;
					}

					std::string keyName =
						capturingKbSettingPtr == ks ? "..." :
						key != 0 ? KeyNames[key] : "None";

					Vector2 keyText(
						sRect.z - textPaddingX - D2D::getTextWidth(keyName, textSize),
						sText.y
					);

					D2D::drawText(sText, setting->name + ":", Color(175, 175, 175), textSize);
					D2D::drawText(keyText, keyName, Color(255, 255, 255), textSize);
				}

				yOffset += textHeight + 1.5f;
			}
		}
	}
}
