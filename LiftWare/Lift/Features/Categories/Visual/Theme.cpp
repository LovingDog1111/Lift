#include "Theme.h"
#include "../../FeatureFactory.h"

Theme::Theme() : Feature("Theme", "Select your client color theme", Category::VISUAL) {
    std::vector<std::string> themes = { "Sunset", "Ocean", "Forest", "Ice" };
    registerSetting(new EnumSetting("Theme", "Select your color theme", themes, &selectedTheme, 0));
    updateColors();
}

void Theme::onD2DRender() {
    updateColors();
}

void Theme::updateColors() {
    switch (selectedTheme) {
    case 0: 
        color1 = Color(255, 30, 0);
        color2 = Color(255, 190, 0);
        break;
    case 1: 
        color1 = Color(33, 150, 243);
        color2 = Color(3, 121, 255);
        break;
    case 2: 
        color1 = Color(46, 125, 50);
        color2 = Color(129, 199, 132);
        break;
    case 3: 
        color1 = Color(122, 233, 255);
        color2 = Color(173, 255, 211);
        break;
    default:
        color1 = Color(255, 233, 255);
        color2 = Color(255, 255, 255);
        break;
    }
}

bool Theme::isEnabled() {
    return true;
}

bool Theme::isVisible() {
    return true;
}