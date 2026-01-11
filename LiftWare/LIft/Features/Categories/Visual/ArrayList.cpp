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

    Color accentColor1 = Colors::getWaveColor(FeatureFactory::getFeature<Theme>()->color1, FeatureFactory::getFeature<Theme>()->color2, 0);
    Color accentColor2 = Colors::getWaveColor(FeatureFactory::getFeature<Theme>()->color1, FeatureFactory::getFeature<Theme>()->color2, 500);

    std::vector<Feature*> modules;
    for (Feature* mod : FeatureFactory::moduleList) {
        if (mod->isEnabled() && mod->isVisible())
            modules.push_back(mod);
    }

    std::sort(modules.begin(), modules.end(), [&](Feature* a, Feature* b) {
        float widthA = D2D::getTextWidth(a->getModuleName(), size) * 0.6f;
        float widthB = D2D::getTextWidth(b->getModuleName(), size) * 0.6f;
        return widthA > widthB;
        });

    float currentY = yOffset;
    float maxRight = 0.f;

    for (size_t i = 0; i < modules.size(); i++) {
        Feature* mod = modules[i];
        std::string moduleName = mod->getModuleName();

        float textWidth = D2D::getTextWidth(moduleName, size);
        float textHeight = D2D::getTextHeight(moduleName, size);

        float rectWidth = textWidth * 0.6f + 6.f;
        float rectLeft = windowSize.x - xMargin - rectWidth;
        float rectRight = windowSize.x - xMargin;
        float rectTop = currentY;
        float rectBottom = rectTop + (textHeight * 0.6f) - (0.02f * size);

        if (rectRight > maxRight)
            maxRight = rectRight;

        Vector4<float> bgRect(rectLeft, rectTop, rectRight, rectBottom);

        D2D::CornerRoundType roundType = D2D::CornerRoundType::BottomOnly;
        if (i == 0)
            roundType = D2D::CornerRoundType::Left;
        else if (i == modules.size() - 1)
            roundType = D2D::CornerRoundType::BottomOnly;

        if (roundType != D2D::CornerRoundType::BottomOnly)
            D2D::fillRectangle(bgRect, Color(0, 0, 0, 125), rounding, roundType);
        else
            D2D::fillBottomLeftRoundedRect(bgRect, Color(0, 0, 0, 125), rounding);

        float textX = rectLeft + (rectWidth - textWidth * 0.6f) / 2.f;
        float textY = rectTop;
        float letterX = textX;

        for (size_t c = 0; c < moduleName.size(); c++) {
            std::string letter = moduleName.substr(c, 1);
            float letterWidth = D2D::getTextWidth(letter, size) * 0.6f;

            float t = (letterX - textX + letterWidth * 0.5f) / (textWidth * 0.6f);
            int r = (int)(accentColor1.r * (1 - t) + accentColor2.r * t);
            int g = (int)(accentColor1.g * (1 - t) + accentColor2.g * t);
            int b = (int)(accentColor1.b * (1 - t) + accentColor2.b * t);
            int a = (int)(accentColor1.a * (1 - t) + accentColor2.a * t);

            D2D::drawText(Vector2<float>(letterX, textY), letter, Color(r, g, b, a), size * 0.6f);
            letterX += letterWidth;
        }

        currentY += (textHeight * 0.6f) - (0.02f * size);
    }

    if (!modules.empty()) {
        Vector4<float> sidelineRect(maxRight + (2.f * size), yOffset, maxRight, currentY);
        D2D::fillGradientRectangleVertical(sidelineRect, accentColor1, accentColor2, 0.f, D2D::CornerRoundType::None);
    }
}
