#include "Colors.h"
#include <math.h>
#include "../Time/Time.h"

uint32_t Colors::ColorToUInt(const Color& color) {
	uint32_t result = (static_cast<uint32_t>(color.a) << 24) |
		(static_cast<uint32_t>(color.r) << 16) |
		(static_cast<uint32_t>(color.g) << 8) |
		color.b;
	return result;
}

std::string Colors::ColorToHexString(const Color& color) {
	std::stringstream ss;
	ss << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(color.r);
	ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(color.g);
	ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(color.b);
	ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(color.a);

	return ss.str();
}

Color Colors::HexStringToColor(std::string hexString) {

	if (hexString.substr(0, 2) == "0x") {
		hexString = hexString.substr(2);
	}


	if (hexString.length() != 8) {
		return Color(0, 0, 0);
	}


	std::stringstream ss;
	ss << std::hex << hexString;

	uint32_t colorValue;
	ss >> colorValue;

	Color color;
	color.r = (uint8_t)((colorValue >> 24) & 0xFF);
	color.g = (uint8_t)((colorValue >> 16) & 0xFF);
	color.b = (uint8_t)((colorValue >> 8) & 0xFF);
	color.a = (uint8_t)(colorValue & 0xFF);

	return color;
}

void Colors::ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b) {
	if (s == 0.0f) {

		out_r = out_g = out_b = v;
		return;
	}

	h = fmodf(h, 1.0f) / (60.0f / 360.0f);
	int i = (int)h;
	float f = h - (float)i;
	float p = v * (1.0f - s);
	float q = v * (1.0f - s * f);
	float t = v * (1.0f - s * (1.0f - f));

	switch (i) {
	case 0:
		out_r = v;
		out_g = t;
		out_b = p;
		break;
	case 1:
		out_r = q;
		out_g = v;
		out_b = p;
		break;
	case 2:
		out_r = p;
		out_g = v;
		out_b = t;
		break;
	case 3:
		out_r = p;
		out_g = q;
		out_b = v;
		break;
	case 4:
		out_r = t;
		out_g = p;
		out_b = v;
		break;
	case 5:
	default:
		out_r = v;
		out_g = p;
		out_b = q;
		break;
	}
}

Color Colors::lerp(const Color& start, const Color& end, float t) {
	Color result;
	result.r = start.r + (int)((end.r - start.r) * t);
	result.g = start.g + (int)((end.g - start.g) * t);
	result.b = start.b + (int)((end.b - start.b) * t);
	result.a = start.a + (int)((end.a - start.a) * t);
	return result;
}

Color Colors::getRainbowColor(float seconds, float saturation, float brightness, long index) {
	float currentHue = (((Time::getCurrentMs() + index) % (int)(seconds * 1000)) / (float)(seconds * 1000));
	float red, green, blue = 0.0f;
	ColorConvertHSVtoRGB(currentHue, saturation, brightness, red, green, blue);

	return Color((int)(red * 255.f), (int)(green * 255.f), (int)(blue * 255.f));
}

Color Colors::getWaveColor(const Color& startColor, const Color& endColor, long index) {
	unsigned __int64 currentMs = Time::getCurrentMs();
	double offset = ((currentMs - index) / 8) / (double)120;
	double aids123 = ((currentMs - index) % 1000 / 1000.000);
	int aids1234 = (int)(((currentMs - index) % 2000 / 2000.000) * 2);
	aids123 = aids1234 % 2 == 0 ? aids123 : 1 - aids123;
	double inverse_percent = 1 - aids123;
	int redPart = (int)(startColor.r * inverse_percent + endColor.r * aids123);
	int greenPart = (int)(startColor.g * inverse_percent + endColor.g * aids123);
	int bluePart = (int)(startColor.b * inverse_percent + endColor.b * aids123);
	int alphaPart = (int)(startColor.a * inverse_percent + endColor.a * aids123);
	return Color(redPart, greenPart, bluePart, alphaPart);
}