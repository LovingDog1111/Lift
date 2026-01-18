#include "ArrayList.h"
#include "../../FeatureFactory.h"

ArrayList::ArrayList() : Feature("ArrayList", "Displays a list of enabled modules in the top-right corner.", Category::VISUAL) {
    registerSetting(new SliderSetting<float>("Size", "Size of the arraylist.", &size, size, 0.f, 2.f));
}

void ArrayList::onD2DRender() {
    Vector2<float> windowSize = D2D::getWindowSize();
    float xMargin = 15.f;
    float yOffset = 15.f;
    float lineThickness = 2.f;

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
    float maxRectWidth = 0.f;
    float maxRectRight = 0.f;
    float totalHeight = 0.f;

    std::vector<Vector4<float>> rects;

    for (size_t i = 0; i < modules.size(); i++) {
        Feature* mod = modules[i];
        std::string moduleName = mod->getModuleName();

        float textWidth = D2D::getTextWidth(moduleName, size);
        float textHeight = D2D::getTextHeight(moduleName, size);

        float rectWidth = textWidth * 0.6f + 6.f;
        if (rectWidth > maxRectWidth) maxRectWidth = rectWidth;

        float rectLeft = windowSize.x - xMargin - rectWidth - lineThickness;
        float rectRight = rectLeft + rectWidth;
        float rectTop = currentY;
        float rectBottom = rectTop + (textHeight * 0.6f);

        if (rectRight > maxRectRight) maxRectRight = rectRight;

        rects.push_back(Vector4<float>(rectLeft, rectTop, rectRight, rectBottom));
        currentY = rectBottom;
        totalHeight = rectBottom - yOffset;
    }

    for (size_t i = 0; i < rects.size(); i++) {
        Vector4<float> rect = rects[i];
        Feature* mod = modules[i];
        std::string moduleName = mod->getModuleName();

        D2D::CornerRoundType roundType = D2D::CornerRoundType::BottomLeftOnly;

        D2D::addDropShadow(rect, 14.0f, Color(0, 0, 0, 255), Vector2<float>(0.0f, 0.0f), 0.f);
        D2D::fillRectangle(rect, Color(0, 0, 0, 255), 0.f, roundType);
        
        float textWidth = D2D::getTextWidth(moduleName, size);
        float textX = rect.x + (rect.z - rect.x - textWidth * 0.6f) / 2.f;
        float textY = rect.y;

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
    }

    if (!modules.empty()) {
        float sidelineX = maxRectRight;
        Vector4<float> sidelineRect(sidelineX, yOffset, sidelineX + lineThickness, yOffset + totalHeight);
        D2D::fillGradientRectangleVertical(sidelineRect, accentColor1, accentColor2, 0.f, D2D::CornerRoundType::None);

        Vector4<float> topLineRect(rects[0].x, yOffset, sidelineX + lineThickness, yOffset + lineThickness);
        D2D::fillGradientRectangle(topLineRect, accentColor1, accentColor2, 0.f, D2D::CornerRoundType::None);
    }
}