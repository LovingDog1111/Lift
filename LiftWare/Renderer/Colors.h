#pragma once
#include <string>
#include <stdint.h>
#include <d2d1.h>
#include "../SDK/Core/mce.h"

struct Color {
	union
	{
		struct {
			uint8_t r, g, b, a;
		};
		int arr[4];
	};

	Color(uint8_t red = 255, uint8_t green = 255, uint8_t blue = 255, uint8_t alpha = 255) {
		this->r = red;
		this->g = green;
		this->b = blue;
		this->a = alpha;
	}

	Color(const mce::Color& color) {
		this->r = (int)(color.r * 255.f);
		this->g = (int)(color.g * 255.f);
		this->b = (int)(color.b * 255.f);
		this->a = (int)(color.a * 255.f);
	}

	bool operator==(const Color& other) const {
		return (r == other.r && g == other.g && b == other.b && a == other.a);
	}

	D2D1_COLOR_F toD2D1Color() const {
		return D2D1_COLOR_F((float)r / 255.f, (float)g / 255.f, (float)b / 255.f, (float)a / 255.f);
	}

	mce::Color toMCColor() const {
		return mce::Color((float)r / 255.f, (float)g / 255.f, (float)b / 255.f, (float)a / 255.f);
	}
};

namespace Colors {
	uint32_t ColorToUInt(const Color& color);
	std::string ColorToHexString(const Color& color);
	Color HexStringToColor(std::string hexString);
	void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);
	Color lerp(const Color& start, const Color& end, float t);
	Color getRainbowColor(float seconds, float saturation, float brightness, long index);
	Color getWaveColor(const Color& startColor, const Color& endColor, long index);
}
