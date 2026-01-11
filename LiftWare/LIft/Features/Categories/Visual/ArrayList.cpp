#include "ArrayList.h"
#include "../../FeatureFactory.h"

ArrayList::ArrayList()
    : Feature("ArrayList", "Displays a list of enabled modules in the top-right corner.", Category::VISUAL)
{
    registerSetting(new SliderSetting<float>("Size", "Size of the arraylist.", &size, size, 0.f, 2.f));
    registerSetting(new SliderSetting<float>("Rounding", "Rounding of the rectangles.", &rounding, rounding, 0.f, 15.f));
}

void ArrayList::onD2DRender() {
    Vector2<float> windowSize = D2D::getWindowSize();
    float xMargin = 15.f;
    float yOffset = 15.f;

    Color accentColor1 = Colors::getWaveColor(Color(255.f, 30.f, 0.f), Color(255.f, 190.f, 0.f), 0);
    Color accentColor2 = Colors::getWaveColor(Color(255.f, 30.f, 0.f), Color(255.f, 190.f, 0.f), 500);

    std::vector<Feature*> modules;
    for (Feature* mod : FeatureFactory::moduleList) {
        if (mod->isEnabled() && mod->isVisible())
            modules.push_back(mod);
    }

    std::sort(modules.begin(), modules.end(), [](Feature* a, Feature* b) {
        return a->getModuleName().length() > b->getModuleName().length();
        });

    float currentY = yOffset;
    float maxRight = 0.f;

    for (size_t i = 0; i < modules.size(); i++) {
        Feature* mod = modules[i];
        std::string moduleName = mod->getModuleName();
        float textWidth = D2D::getTextWidth(moduleName, size) * 0.8f;
        float textHeight = D2D::getTextHeight(moduleName, size) * 0.8f;

        float rectWidth = textWidth + 6.f;
        float rectLeft = windowSize.x - xMargin - rectWidth;
        float rectRight = windowSize.x - xMargin;
        float rectTop = currentY;
        float rectBottom = rectTop + textHeight;

        if (rectRight > maxRight)
            maxRight = rectRight;

        Vector4<float> bgRect(rectLeft, rectTop, rectRight, rectBottom);

        D2D::CornerRoundType roundType = D2D::CornerRoundType::BottomOnly;
        if (i == 0)
            roundType = D2D::CornerRoundType::Left;
        else if (i == modules.size() - 1)
            roundType = D2D::CornerRoundType::BottomOnly;

        if (roundType != D2D::CornerRoundType::BottomOnly) {
            D2D::fillRectangle(bgRect, Color(0, 0, 0, 125), 6.f, roundType);
        }
        else {
            D2D::fillBottomLeftRoundedRect(bgRect, Color(0, 0, 0, 125), 6.f);
        }

        float textX = rectLeft + (rectWidth - textWidth) / 2.f;
        float textY = rectTop;
        float letterX = textX;

        for (size_t c = 0; c < moduleName.size(); c++) {
            std::string letter = moduleName.substr(c, 1);
            float letterWidth = D2D::getTextWidth(letter, size) * 0.8f;

            float t = (letterX - textX + letterWidth * 0.5f) / textWidth;
            int r = (int)(accentColor1.r * (1 - t) + accentColor2.r * t);
            int g = (int)(accentColor1.g * (1 - t) + accentColor2.g * t);
            int b = (int)(accentColor1.b * (1 - t) + accentColor2.b * t);
            int a = (int)(accentColor1.a * (1 - t) + accentColor2.a * t);

            D2D::drawText(Vector2<float>(letterX, textY), letter, Color(r, g, b, a), size * 0.8f);
            letterX += letterWidth;
        }

        currentY = rectBottom;
    }

    if (!modules.empty()) {
        Vector4<float> sidelineRect(maxRight + (2.f * size), yOffset, maxRight, currentY);
        D2D::fillGradientRectangleVertical(sidelineRect, accentColor1, accentColor2, 0.f, D2D::CornerRoundType::None);
    }
}
