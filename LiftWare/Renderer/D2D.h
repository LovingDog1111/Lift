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
	extern float deltaTime;

	void NewFrame(IDXGISwapChain3* swapChain, ID3D11Device* d3d11Device, float fxdpi);
	void EndFrame();
	void Render();
	void Clean();
	void Flush();

	Vector2<float> getWindowSize();
	void drawText(const Vector2<float>& textPos, const std::string& textStr, const Color& color, float textSize = 1.f, bool storeTextLayout = true);
	float getTextWidth(const std::string& textStr, float textSize = 1.f, bool storeTextLayout = true);
	float getTextHeight(const std::string& textStr, float textSize = 1.f, bool storeTextLayout = true);
	void drawLine(const Vector2<float>& startPos, const Vector2<float>& endPos, const Color& color, float width = 1.f);
	void drawRectangle(const Vector4<float>& rect, const Color& color, float width = 1.f);
	void fillRectangle(const Vector4<float>& rect, const Color& color);
	void drawCircle(const Vector2<float>& centerPos, const Color& color, float radius, float width = 1.f);
	void fillCircle(const Vector2<float>& centerPos, const Color& color, float radius);
	void addBlur(const Vector4<float>& rect, float strength, bool flush = true);
	void fillTriangle(const Vector2<float>& p1, const Vector2<float>& p2, const Vector2<float>& p3, const Color& color);
	void drawTriangle(const Vector2<float>& p1, const Vector2<float>& p2, const Vector2<float>& p3, const Color& color, float width = 1.0f);
	void fillRoundedRect(const Vector4<float>& rect, const Color& color, float radius);
	void drawRoundedRect(const Vector4<float>& rect, const Color& color, float radius, float width = 1.0f);
	void beginClip(const Vector4<float>& clipRect);
	void endClip();
};
