#pragma once
#include "../../FeatureBase/FeatureBase.h"

class ClickGUI : public Feature {

private:
    struct Header {
        std::string name;
        Category category;
        Vector2<float> pos;
        bool extended = true;
        std::vector<Feature*> moduleList;

        Header(std::string windowName, Vector2<float> startPos, Category c);
    };
    KeybindSetting* capturingKbSettingPtr = nullptr;
public:
    ClickGUI();

    void onDisable() override;
    void onEnable() override;
    bool isVisible() override;
    void onKeyUpdate(int key, bool isDown) override;
    void onMouseUpdate(Vector2<float> mousePos, char mouseButton, char isDown) override;

    void InitClickGUI();
    void onD2DRender() override;
private:
    Vector2<float> mousePos;
    Vector2<float> mouseDelta = Vector2<float>(0.f, 0.f);
    bool isLeftClickDown;
    bool isRightClickDown;
    bool isHoldingLeftClick;
    bool isHoldingRightClick;
public:
    std::vector<Header*> windowList;
    Header* draggingWindowPtr = nullptr;
    bool initialized = false;
};