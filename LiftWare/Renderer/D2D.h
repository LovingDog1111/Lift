#pragma once
#include <d3d11.h>
#include <d3d12.h>
#include <d3d11on12.h>
#include <d2d1_3.h>
#include <dxgi1_4.h>
#include <dwrite_3.h>

#include <string>
#include "../Memory/Memory.h"
#include "../Math/IncludeAll.h"
#include "Colors.h"

namespace D2D {
	static const Color GLOW_USE_MAIN_COLOR;
	extern float deltaTime;
	enum class CornerRoundType {
		None,
		BottomOnly,
		TopOnly,
		Full,
		SidesOnly,
		Left,
		Right
	};

	void NewFrame(IDXGISwapChain3* swapChain, ID3D11Device* d3d11Device, float fxdpi);
	void EndFrame();
	void Render();
	void Clean();
	void Flush();

	Vector2<float> getWindowSize();
	void drawText(const Vector2<float>& textPos, const std::string& textStr, const Color& color, float textSize = 1.f, bool storeTextLayout = true, bool bold = false);
	float getTextWidth(const std::string& textStr, float textSize = 1.f, bool storeTextLayout = true, bool bold = false);
	float getTextHeight(const std::string& textStr, float textSize = 1.f, bool storeTextLayout = true, bool bold = false);
	void drawLine(const Vector2<float>& startPos, const Vector2<float>& endPos, const Color& color, float width = 1.f);
	void drawRectangle(const Vector4<float>& rect, const Color& color, float width = 1.0f, float rounding = 0.0f, CornerRoundType roundType = CornerRoundType::None);
	void fillRectangle(const Vector4<float>& rect, const Color& color, float rounding = 0.0f, CornerRoundType roundType = CornerRoundType::None);
	void drawCircle(const Vector2<float>& centerPos, const Color& color, float radius, float width = 1.f);
	void fillCircle(const Vector2<float>& centerPos, const Color& color, float radius);
	void addBlur(const Vector4<float>& rect, float strength, bool flush = true);
	void fillTriangle(const Vector2<float>& p1, const Vector2<float>& p2, const Vector2<float>& p3, const Color& color);
	void drawTriangle(const Vector2<float>& p1, const Vector2<float>& p2, const Vector2<float>& p3, const Color& color, float width = 1.0f);
	void fillBottomLeftRoundedRect(const Vector4<float>& rect, const Color& color, float radius);
	void fillGradientRectangleVertical(const Vector4<float>& rect, const Color& startColor, const Color& endColor, float rounding, CornerRoundType roundType);
	void fillGradientRectangle(const Vector4<float>& rect, const Color& startColor, const Color& endColor, float rounding, CornerRoundType roundType);
	void beginClip(const Vector4<float>& clipRect);
	void endClip();
	void drawGlowingCircle(const Vector2<float>& center, float radius, const Color& color, float glowRadius, float outlineThickness = 1.0f);
	void fillGlowingCircle(const Vector2<float>& center, float radius, const Color& color, float glowRadius);
	void drawNeuralNetworkParticlesOptimized(
		const Vector2<float>& screenSize,
		const Color& color,
		int particleCount,
		float particleRadius,
		float lineWidth,
		float maxDistance
	);
	void drawStarParticles(const Vector2<float>& screenSize, const Color& color, int particleCount, float speed, float size);
};
