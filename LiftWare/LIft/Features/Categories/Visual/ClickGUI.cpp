#include "ClickGUI.h"
#include "../../FeatureFactory.h"

ClickGUI::ClickGUI() : Feature("ClickGUI", "A module to display features.", Category::VISUAL, VK_INSERT) {

}

ClickGUI::Header::Header(std::string windowName, Vector2<float> startPos, Category c) {
    this->name = windowName;
    this->category = c;
    this->pos = startPos;
    this->extended = true;
    this->animProgress = 1.f;

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
    for (Feature* mod : FeatureFactory::moduleList) {
        mod->animProgress = 0.0f;
    }
}

void ClickGUI::onEnable() {
    Game::clientInstance->releaseMouse();
    for (Feature* mod : FeatureFactory::moduleList) {
        mod->animProgress = 0.0f;
    }
}

bool ClickGUI::isVisible() {
    return false;
}

void ClickGUI::onKeyUpdate(int key, bool isDown) {
    if (!initialized)
        return;

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
    if (!initialized)
        return;

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

    Vector2<float> screenSize = Game::clientInstance->guiData->windowSizeReal;
    float windowWidth = 200.f;
    float windowHeight = 20.f;
    int numWindows = 6;
    float totalWidth = numWindows * windowWidth;
    float totalHeight = windowHeight;

    float startX = (screenSize.x - totalWidth) * 0.5f;
    float startY = screenSize.y - 600.f;

    Vector2<float> startPos(startX, startY);

    windowList.push_back(new Header("Combat", startPos, Category::COMBAT));
    startPos.x += windowWidth;
    windowList.push_back(new Header("Movement", startPos, Category::MOVEMENT));
    startPos.x += windowWidth;
    windowList.push_back(new Header("Visual", startPos, Category::VISUAL));
    startPos.x += windowWidth;
    windowList.push_back(new Header("Player", startPos, Category::PLAYER));
    startPos.x += windowWidth;
    windowList.push_back(new Header("Exploit", startPos, Category::EXPLOIT));
    startPos.x += windowWidth;
    windowList.push_back(new Header("Other", startPos, Category::OTHER));

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
    Color accentColor1 = Colors::getWaveColor(Color(255.f, 30.f, 0.f), Color(255.f, 190.f, 0.f), 0);
    Color accentColor2 = Colors::getWaveColor(Color(255.f, 30.f, 0.f), Color(255.f, 190.f, 0.f), 500);

    Vector2<float> screenSize = Game::clientInstance->guiData->windowSizeReal;
    D2D::fillRectangle(Vector4<float>(0.f, 0.f, screenSize.x, screenSize.y), Color(0, 0, 0, 125));
    D2D::addBlur(Vector4<float>(0.f, 0.f, screenSize.x, screenSize.y), 3.f);

    D2D::fillGradientRectangleVertical(Vector4<float>(0.f, 0.f, screenSize.x, screenSize.y), Color(accentColor1.r, accentColor1.g, accentColor1.b, 100.f), Color(0.f, 0.f, 0.f, 20.f), 0.f, D2D::CornerRoundType::None);

    D2D::drawStarParticles(screenSize, accentColor1, 167, 0.1f, 5.f);

    float textSize = 0.6f;
    float textPaddingX = 5.f * textSize;
    float textHeight = D2D::getTextHeight("X", textSize * 1.65f);

    float headerTextSize = 0.75f;
    float headerTextHeight = D2D::getTextHeight("X", textSize * 1.75f);

    float rounding = 6.f;
    float animSpeed = 20.f;

    Color Gray = Color(35.f, 35.f, 35.f);
    Color SolsticeBlack = Color(0.f, 0.f, 0.f);

    for (auto& window : windowList) {
        if (draggingWindowPtr == window)
            window->pos = window->pos + mouseDelta;

        float desiredWidth = 185.f + textPaddingX * 2.f;
        float headerWidth = desiredWidth;

        float clampedX = window->pos.x;
        if (clampedX + headerWidth > screenSize.x)
            clampedX = screenSize.x - headerWidth;
        if (clampedX < 0.f)
            clampedX = 0.f;

        Vector4<float> hRect(clampedX, window->pos.y, clampedX + headerWidth, window->pos.y + headerTextHeight);
        Vector2 hText(hRect.x + (headerWidth - D2D::getTextWidth(window->name, headerTextSize, true, true)) * 0.5f,
            hRect.y + (headerTextHeight - D2D::getTextHeight("X", headerTextSize, true, true)) * 0.5f);

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

        window->animProgress += (window->extended ? 0.3f : -0.3f) * (D2D::deltaTime * animSpeed);
        if (window->animProgress > 1.f) window->animProgress = 1.f;
        if (window->animProgress < 0.f) window->animProgress = 0.f;

        D2D::fillRectangle(hRect, SolsticeBlack, rounding, D2D::CornerRoundType::TopOnly);
        D2D::drawText(hText, window->name, Color(255.f, 255.f, 255.f), headerTextSize, true, true);

        Vector4<float> underlineRect(hRect.x, hRect.w - 2.0f, hRect.z, hRect.w);
        D2D::fillGradientRectangle(underlineRect, accentColor1, accentColor2, 1.0f, D2D::CornerRoundType::None);

        if (window->animProgress <= 0.f)
            continue;

        float yOffset = hRect.w;

        for (size_t i = 0; i < window->moduleList.size(); i++) {
            auto& mod = window->moduleList[i];

            if (!window->extended) {
                mod->extended = false;
            }

            mod->animProgress += (mod->extended ? 0.3f : -0.3f) * (D2D::deltaTime * animSpeed);
            if (mod->animProgress > 1.f) mod->animProgress = 1.f;
            if (mod->animProgress < 0.f) mod->animProgress = 0.f;

            mod->enableAnimProgress += (mod->isEnabled() ? 0.3f : -0.3f) * (D2D::deltaTime * animSpeed);
            if (mod->enableAnimProgress > 1.f) mod->enableAnimProgress = 1.f;
            if (mod->enableAnimProgress < 0.f) mod->enableAnimProgress = 0.f;

            Vector4 mRect(hRect.x, yOffset, hRect.z, yOffset + textHeight * window->animProgress);

            float animHeight = textHeight * window->animProgress;
            if (animHeight <= 0.f) {
                yOffset += textHeight * window->animProgress;
                continue;
            }

            Vector4 clipRect(mRect.x, mRect.y, mRect.z, mRect.y + animHeight);
            D2D::beginClip(clipRect);

            Vector2 mText(
                mRect.x + (mRect.z - mRect.x - D2D::getTextWidth(mod->getModuleName(), textSize)) * 0.5f,
                mRect.y + (animHeight - D2D::getTextHeight("X", textSize)) * 0.5f
            );

            if (window->animProgress >= 1.f && mRect.contains(mousePos)) {
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

            bool isLastModule = (i == window->moduleList.size() - 1);
            float moduleRounding = (isLastModule && mod->animProgress <= 0.f && window->animProgress >= 1.f) ? rounding : 0.f;

            Color startColor = Colors::lerp(Gray, accentColor1, mod->enableAnimProgress);
            Color endColor = Colors::lerp(Gray, accentColor2, mod->enableAnimProgress);
            Color textColor = Colors::lerp(Color(175.f, 175.f, 175.f), Color(255.f, 255.f, 255.f), mod->enableAnimProgress);

            D2D::fillGradientRectangle(
                Vector4<float>(mRect.x, mRect.y, mRect.z, mRect.y + textHeight),
                startColor,
                endColor,
                moduleRounding,
                moduleRounding > 0.f ? D2D::CornerRoundType::BottomOnly : D2D::CornerRoundType::None
            );

            D2D::drawText(
                mText,
                mod->getModuleName(),
                textColor,
                textSize
            );

            D2D::endClip();

            yOffset += textHeight * window->animProgress;

            if (mod->animProgress > 0.f && window->animProgress >= 1.f) {
                float centerX = mRect.x + (mRect.z - mRect.x) * 0.5f;
                float startX = centerX - ((mRect.z - mRect.x) * 0.5f * mod->animProgress);
                float endX = centerX + ((mRect.z - mRect.x) * 0.5f * mod->animProgress);
                Vector4<float> underlineRect(startX, mRect.y + textHeight - 2.f, endX, mRect.y + textHeight);
                Vector4 underlineClip(mRect.x, mRect.y, mRect.z, mRect.y + textHeight);
                D2D::beginClip(underlineClip);
                D2D::fillGradientRectangle(underlineRect, accentColor1, accentColor2, 1.f, D2D::CornerRoundType::None);
                D2D::endClip();
            }

            if (mod->animProgress <= 0.f || window->animProgress < 1.f) {
                continue;
            }

            auto& settings = mod->getSettingList();
            for (size_t j = 0; j < settings.size(); j++) {
                auto& setting = settings[j];
                if (setting->type != SettingType::BOOL && setting->type != SettingType::KEYBIND)
                    continue;

                Vector4 sRect(
                    hRect.x,
                    yOffset,
                    hRect.z,
                    yOffset + textHeight * mod->animProgress
                );

                float settingAnimHeight = textHeight * mod->animProgress;
                if (settingAnimHeight <= 0.f) {
                    yOffset += textHeight * mod->animProgress;
                    continue;
                }

                Vector4 settingClipRect(sRect.x, sRect.y, sRect.z, sRect.y + settingAnimHeight);
                D2D::beginClip(settingClipRect);

                Vector2 sText(
                    sRect.x + textPaddingX,
                    sRect.y + (settingAnimHeight - D2D::getTextHeight("X", textSize)) * 0.5f
                );

                if (sRect.contains(mousePos))
                    descriptionText = setting->description;

                bool isLastSetting = (j == settings.size() - 1);

                D2D::fillRectangle(
                    Vector4<float>(sRect.x, sRect.y, sRect.z, sRect.y + textHeight),
                    Gray,
                    isLastSetting ? rounding : 0.f,
                    isLastSetting ? D2D::CornerRoundType::BottomOnly : D2D::CornerRoundType::None
                );

                if (setting->type == SettingType::BOOL) {
                    auto* bs = static_cast<BoolSetting*>(setting);
                    bool& value = *bs->value;

                    bs->boolAnimProgress += (value ? 0.3f : -0.3f) * (D2D::deltaTime * animSpeed);
                    if (bs->boolAnimProgress > 1.f) bs->boolAnimProgress = 1.f;
                    if (bs->boolAnimProgress < 0.f) bs->boolAnimProgress = 0.f;

                    if (sRect.contains(mousePos) && isLeftClickDown) {
                        value = !value;
                        isLeftClickDown = false;
                    }

                    Color textColor = Colors::lerp(Color(255.f, 255.f, 255.f), Color(175.f, 175.f, 175.f), bs->boolAnimProgress);

                    D2D::drawText(
                        sText,
                        setting->name,
                        textColor,
                        textSize
                    );

                    float circleRadius = textHeight * 0.2f;
                    Vector2<float> circleCenter(
                        sRect.z - circleRadius - textPaddingX,
                        sRect.y + textHeight * 0.5f
                    );

                    if (value || bs->boolAnimProgress > 0.f) {
                        Color glowColor = accentColor1;
                        glowColor.a = 255.f * bs->boolAnimProgress;

                        D2D::drawGlowingCircle(circleCenter, circleRadius, glowColor, 3.f * bs->boolAnimProgress, 2.f * bs->boolAnimProgress);
                        D2D::drawCircle(circleCenter, glowColor, circleRadius, 1.0f * bs->boolAnimProgress);
                    }
                    else {
                        Color glowColor = accentColor1;
                        glowColor.a = 255.f;
                        D2D::fillGlowingCircle(circleCenter, circleRadius, glowColor, 3.f);
                    }
                }

                if (setting->type == SettingType::KEYBIND) {
                    auto* ks = static_cast<KeybindSetting*>(setting);
                    int& key = *ks->value;

                    if (sRect.contains(mousePos) && isLeftClickDown) {
                        capturingKbSettingPtr = (capturingKbSettingPtr == ks ? nullptr : ks);
                        isLeftClickDown = false;
                    }

                    std::string keyName = capturingKbSettingPtr == ks ? "..." : key != 0 ? KeyNames[key] : "None";
                    Vector2 keyText(sRect.z - textPaddingX - D2D::getTextWidth(keyName, textSize), sText.y);

                    D2D::drawText(sText, setting->name + ":", Color(175.f, 175.f, 175.f), textSize);
                    D2D::drawText(keyText, keyName, Color(255.f, 255.f, 255.f), textSize);
                }

                D2D::endClip();

                yOffset += textHeight * mod->animProgress;
            }
        }
    }
}