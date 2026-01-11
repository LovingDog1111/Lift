#include "ArrayList.h"
#include "../../FeatureFactory.h"

ArrayList::ArrayList()
    : Feature("ArrayList", "Displays a list of enabled modules in the top-right corner.", Category::VISUAL)
{
}

void ArrayList::onD2DRender() {
    Vector2<float> windowSize = D2D::getWindowSize();
    float xMargin = 15.f;
    float yOffset = 15.f;

    std::vector<Feature*> modules;

    for (Feature* mod : FeatureFactory::moduleList) {
        if (mod->isEnabled() && mod->isVisible())
            modules.push_back(mod);
    }

    std::sort(modules.begin(), modules.end(), [](Feature* a, Feature* b) {
        return a->getModuleName().length() > b->getModuleName().length();
        });

    float heightOffset = 0.f;
    int index = 0;

    for (size_t i = 0; i < modules.size(); i++) {
        index++;

        Feature* mod = modules[i];
        std::string moduleName = mod->getModuleName();
        float textWidth = D2D::getTextWidth(moduleName, 1.f) * 0.8f;
        float textHeight = D2D::getTextHeight(moduleName, 1.f) * 0.8f;

        float originalWidth = D2D::getTextWidth(moduleName, 1.f) + 6.f;
        float rectWidth = originalWidth * 0.9f; 
        float rectLeft = windowSize.x - xMargin - rectWidth;
        float rectRight = windowSize.x - xMargin;
        float rectTop = yOffset + heightOffset;
        float rectBottom = rectTop + D2D::getTextHeight(moduleName, 1.f) + 2.f;

        Vector4<float> bgRect(rectLeft, rectTop, rectRight, rectBottom);
        D2D::fillRoundedRect(bgRect, Color(0, 0, 0, 125), 3.f);

        Color rainbow = Colors::getRainbowColor(4.f, 1.f, 1.f, index * 125);
        D2D::fillRectangle(Vector4<float>(rectLeft + 2.f, rectTop, rectLeft, rectBottom), rainbow);

        float textX = rectLeft + ((rectRight - rectLeft) - textWidth) / 2.f;
        float textY = rectTop + ((rectBottom - rectTop) - textHeight) / 2.f;
        D2D::drawText(Vector2<float>(textX, textY), moduleName, Color(255, 255, 255, 255), 0.8f);

        heightOffset += (rectBottom - rectTop) + 2.f;
    }
}
