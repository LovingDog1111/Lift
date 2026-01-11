#include "D2D.h"
#include <unordered_map>
#include <winrt/base.h>
#include "../MainClient/Lift.h"
#include "../MainClient/Features/FeatureFactory.h"
#include "../SDK/Game.h"
#include "../Time/Time.h"
#include "../Logs/Logger.h"

float D2D::deltaTime = 0.016f;


static ID2D1Factory3* d2dFactory = nullptr;
static IDWriteFactory* d2dWriteFactory = nullptr;
static ID2D1Device2* d2dDevice = nullptr;
static ID2D1DeviceContext2* d2dDeviceContext = nullptr;
static ID2D1Bitmap1* sourceBitmap = nullptr;
static ID2D1Effect* blurEffect = nullptr;


static std::unordered_map<float, winrt::com_ptr<IDWriteTextFormat>> textFormatCache;
static std::unordered_map<uint64_t, winrt::com_ptr<IDWriteTextLayout>> textLayoutCache;
static std::unordered_map<uint32_t, winrt::com_ptr<ID2D1SolidColorBrush>> colorBrushCache;


static std::unordered_map<uint64_t, winrt::com_ptr<IDWriteTextLayout>> textLayoutTemporary;

static int currentD2DFontSize = 20;
static std::string currentD2DFont = "Mojang";
static bool isFontItalic = false;

static bool initD2D = false;

template<typename T>
void SafeRelease(T*& ptr) {
	if (ptr != nullptr) {
		ptr->Release();
		ptr = nullptr;
	}
}

std::wstring to_wide(const std::string& str);
uint64_t getTextLayoutKey(const std::string& textStr, float textSize);
IDWriteTextFormat* getTextFormat(float textSize);
IDWriteTextLayout* getTextLayout(const std::string& textStr, float textSize, bool storeTextLayout = true, bool bold = false);
ID2D1SolidColorBrush* getSolidColorBrush(const Color& color);

void D2D::NewFrame(IDXGISwapChain3* swapChain, ID3D11Device* d3d11Device, float fxdpi) {
	if (!initD2D) {

		D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &d2dFactory);

		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(d2dWriteFactory), reinterpret_cast<IUnknown**>(&d2dWriteFactory));

		IDXGIDevice* dxgiDevice;
		d3d11Device->QueryInterface<IDXGIDevice>(&dxgiDevice);
		d2dFactory->CreateDevice(dxgiDevice, &d2dDevice);
		dxgiDevice->Release();

		d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dDeviceContext);


		d2dDeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &blurEffect);
		blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
		blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_QUALITY);

		IDXGISurface* dxgiBackBuffer = nullptr;
		swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));
		D2D1_BITMAP_PROPERTIES1 bitmapProperties
			= D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), fxdpi, fxdpi);
		d2dDeviceContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer, &bitmapProperties, &sourceBitmap);
		dxgiBackBuffer->Release();
		d2dDeviceContext->SetTarget(sourceBitmap);

		initD2D = true;
	}

	d2dDeviceContext->BeginDraw();
}

void D2D::EndFrame() {
	if (!initD2D)
		return;

	d2dDeviceContext->EndDraw();

	if ((currentD2DFont != currentD2DFont) || (currentD2DFontSize != 25) || (isFontItalic != false)) {
		currentD2DFont = currentD2DFont;
		currentD2DFontSize = 25;
		isFontItalic = false;
		textFormatCache.clear();
		textLayoutCache.clear();

	}

	static float timeCounter = 0.0f;
	timeCounter += D2D::deltaTime;
	if (timeCounter > 90.f) {
		if (textFormatCache.size() > 1000)
			textFormatCache.clear();

		if (textLayoutCache.size() > 500)
			textLayoutCache.clear();

		if (colorBrushCache.size() > 2000)
			colorBrushCache.clear();

		timeCounter = 0.0f;
	}

	textLayoutTemporary.clear();
}

void D2D::Render() {
	FeatureFactory::onD2DRender();
}

void D2D::Clean() {
	if (!initD2D)
		return;

	SafeRelease(d2dFactory);
	SafeRelease(d2dWriteFactory);
	SafeRelease(d2dDevice);
	SafeRelease(d2dDeviceContext);
	SafeRelease(sourceBitmap);
	SafeRelease(blurEffect);

	textFormatCache.clear();
	textLayoutCache.clear();
	colorBrushCache.clear();

	textLayoutTemporary.clear();

	initD2D = false;
}

void D2D::Flush() {
	d2dDeviceContext->Flush();
}

Vector2<float> D2D::getWindowSize() {
	D2D1_SIZE_U size = sourceBitmap->GetPixelSize();
	return Vector2<float>((float)size.width, (float)size.height);
}

void D2D::drawText(const Vector2<float>& textPos, const std::string& textStr, const Color& color, float textSize, bool storeTextLayout, bool bold) {
	IDWriteTextLayout* textLayout = getTextLayout(textStr, textSize, storeTextLayout, bold);

	ID2D1SolidColorBrush* shadowColorBrush = getSolidColorBrush(Color(0, 0, 0, color.a));
	d2dDeviceContext->DrawTextLayout(D2D1::Point2F(textPos.x + 2.f, textPos.y + 2.f), textLayout, shadowColorBrush);

	ID2D1SolidColorBrush* colorBrush = getSolidColorBrush(color);
	d2dDeviceContext->DrawTextLayout(D2D1::Point2F(textPos.x, textPos.y), textLayout, colorBrush);
}

float D2D::getTextWidth(const std::string& textStr, float textSize, bool storeTextLayout, bool bold) {
	IDWriteTextLayout* textLayout = getTextLayout(textStr, textSize, storeTextLayout, bold);
	DWRITE_TEXT_METRICS textMetrics;
	textLayout->GetMetrics(&textMetrics);

	return textMetrics.widthIncludingTrailingWhitespace;
}

float D2D::getTextHeight(const std::string& textStr, float textSize, bool storeTextLayout, bool bold) {
	IDWriteTextLayout* textLayout = getTextLayout(textStr, textSize, storeTextLayout, bold);
	DWRITE_TEXT_METRICS textMetrics;
	textLayout->GetMetrics(&textMetrics);

	return std::ceilf(textMetrics.height);
}

void D2D::drawLine(const Vector2<float>& startPos, const Vector2<float>& endPos, const Color& color, float width) {
	ID2D1SolidColorBrush* colorBrush = getSolidColorBrush(color);
	d2dDeviceContext->DrawLine(D2D1::Point2F(startPos.x, startPos.y), D2D1::Point2F(endPos.x, endPos.y), colorBrush, width);
}

void D2D::drawRectangle(const Vector4<float>& rect, const Color& color, float width, float rounding, CornerRoundType roundType) {
	ID2D1SolidColorBrush* brush = getSolidColorBrush(color);

	if (rounding <= 0.0f || roundType == CornerRoundType::None) {
		d2dDeviceContext->DrawRectangle(D2D1::RectF(rect.x, rect.y, rect.z, rect.w), brush, width);
		return;
	}

	if (roundType == CornerRoundType::Full) {
		D2D1_ROUNDED_RECT roundedRect = { D2D1::RectF(rect.x, rect.y, rect.z, rect.w), rounding, rounding };
		d2dDeviceContext->DrawRoundedRectangle(&roundedRect, brush, width);
		return;
	}

	ID2D1PathGeometry* geometry = nullptr;
	ID2D1GeometrySink* sink = nullptr;
	d2dFactory->CreatePathGeometry(&geometry);
	geometry->Open(&sink);

	float x1 = rect.x, y1 = rect.y, x2 = rect.z, y2 = rect.w;

	bool topLeft = roundType == CornerRoundType::TopOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Left;
	bool topRight = roundType == CornerRoundType::TopOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Right;
	bool bottomLeft = roundType == CornerRoundType::BottomOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Left;
	bool bottomRight = roundType == CornerRoundType::BottomOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Right;

	sink->BeginFigure(D2D1::Point2F(x1 + (topLeft ? rounding : 0), y1), D2D1_FIGURE_BEGIN_HOLLOW);

	sink->AddLine(D2D1::Point2F(x2 - (topRight ? rounding : 0), y1));
	if (topRight) sink->AddArc(D2D1::ArcSegment(
		D2D1::Point2F(x2, y1 + rounding),
		D2D1::SizeF(rounding, rounding), 0.0f,
		D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

	sink->AddLine(D2D1::Point2F(x2, y2 - (bottomRight ? rounding : 0)));
	if (bottomRight) sink->AddArc(D2D1::ArcSegment(
		D2D1::Point2F(x2 - rounding, y2),
		D2D1::SizeF(rounding, rounding), 0.0f,
		D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

	sink->AddLine(D2D1::Point2F(x1 + (bottomLeft ? rounding : 0), y2));
	if (bottomLeft) sink->AddArc(D2D1::ArcSegment(
		D2D1::Point2F(x1, y2 - rounding),
		D2D1::SizeF(rounding, rounding), 0.0f,
		D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

	sink->AddLine(D2D1::Point2F(x1, y1 + (topLeft ? rounding : 0)));
	if (topLeft) sink->AddArc(D2D1::ArcSegment(
		D2D1::Point2F(x1 + rounding, y1),
		D2D1::SizeF(rounding, rounding), 0.0f,
		D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

	sink->EndFigure(D2D1_FIGURE_END_OPEN);
	sink->Close();
	sink->Release();

	d2dDeviceContext->DrawGeometry(geometry, brush, width);
	geometry->Release();
}

void D2D::fillRectangle(const Vector4<float>& rect, const Color& color, float rounding, CornerRoundType roundType) {
	ID2D1SolidColorBrush* brush = getSolidColorBrush(color);

	if (rounding <= 0.0f || roundType == CornerRoundType::None) {
		d2dDeviceContext->FillRectangle(D2D1::RectF(rect.x, rect.y, rect.z, rect.w), brush);
		return;
	}

	if (roundType == CornerRoundType::Full) {
		D2D1_ROUNDED_RECT roundedRect = { D2D1::RectF(rect.x, rect.y, rect.z, rect.w), rounding, rounding };
		d2dDeviceContext->FillRoundedRectangle(&roundedRect, brush);
		return;
	}

	ID2D1PathGeometry* geometry = nullptr;
	ID2D1GeometrySink* sink = nullptr;
	d2dFactory->CreatePathGeometry(&geometry);
	geometry->Open(&sink);

	float x1 = rect.x, y1 = rect.y, x2 = rect.z, y2 = rect.w;

	bool topLeft = roundType == CornerRoundType::TopOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Left;
	bool topRight = roundType == CornerRoundType::TopOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Right;
	bool bottomLeft = roundType == CornerRoundType::BottomOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Left;
	bool bottomRight = roundType == CornerRoundType::BottomOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Right;

	sink->BeginFigure(D2D1::Point2F(x1 + (topLeft ? rounding : 0), y1), D2D1_FIGURE_BEGIN_FILLED);

	sink->AddLine(D2D1::Point2F(x2 - (topRight ? rounding : 0), y1));
	if (topRight) sink->AddArc(D2D1::ArcSegment(
		D2D1::Point2F(x2, y1 + rounding),
		D2D1::SizeF(rounding, rounding), 0.0f,
		D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

	sink->AddLine(D2D1::Point2F(x2, y2 - (bottomRight ? rounding : 0)));
	if (bottomRight) sink->AddArc(D2D1::ArcSegment(
		D2D1::Point2F(x2 - rounding, y2),
		D2D1::SizeF(rounding, rounding), 0.0f,
		D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

	sink->AddLine(D2D1::Point2F(x1 + (bottomLeft ? rounding : 0), y2));
	if (bottomLeft) sink->AddArc(D2D1::ArcSegment(
		D2D1::Point2F(x1, y2 - rounding),
		D2D1::SizeF(rounding, rounding), 0.0f,
		D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

	sink->AddLine(D2D1::Point2F(x1, y1 + (topLeft ? rounding : 0)));
	if (topLeft) sink->AddArc(D2D1::ArcSegment(
		D2D1::Point2F(x1 + rounding, y1),
		D2D1::SizeF(rounding, rounding), 0.0f,
		D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

	sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	sink->Close();
	sink->Release();

	d2dDeviceContext->FillGeometry(geometry, brush);
	geometry->Release();
}

void D2D::drawCircle(const Vector2<float>& centerPos, const Color& color, float radius, float width) {
	ID2D1SolidColorBrush* colorBrush = getSolidColorBrush(color);
	d2dDeviceContext->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(centerPos.x, centerPos.y), radius, radius), colorBrush, width);
}

void D2D::fillCircle(const Vector2<float>& centerPos, const Color& color, float radius) {
	ID2D1SolidColorBrush* colorBrush = getSolidColorBrush(color);
	d2dDeviceContext->FillEllipse(D2D1::Ellipse(D2D1::Point2F(centerPos.x, centerPos.y), radius, radius), colorBrush);
}

void D2D::addBlur(const Vector4<float>& rect, float strength, bool flush) {
	if (flush) {
		d2dDeviceContext->Flush();
	}
	ID2D1Bitmap* targetBitmap = nullptr;
	D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(sourceBitmap->GetPixelFormat());
	d2dDeviceContext->CreateBitmap(sourceBitmap->GetPixelSize(), props, &targetBitmap);
	D2D1_POINT_2U destPoint = D2D1::Point2U(0, 0);
	D2D1_SIZE_U size = sourceBitmap->GetPixelSize();
	D2D1_RECT_U Rect = D2D1::RectU(0, 0, size.width, size.height);
	targetBitmap->CopyFromBitmap(&destPoint, sourceBitmap, &Rect);

	D2D1_RECT_F screenRectF = D2D1::RectF(0.f, 0.f, (float)sourceBitmap->GetPixelSize().width, (float)sourceBitmap->GetPixelSize().height);
	D2D1_RECT_F clipRectD2D = D2D1::RectF(rect.x, rect.y, rect.z, rect.w);

	blurEffect->SetInput(0, targetBitmap);
	blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, strength);

	ID2D1Image* outImage = nullptr;
	blurEffect->GetOutput(&outImage);

	ID2D1ImageBrush* outImageBrush = nullptr;
	D2D1_IMAGE_BRUSH_PROPERTIES outImage_props = D2D1::ImageBrushProperties(screenRectF);
	d2dDeviceContext->CreateImageBrush(
		outImage,
		outImage_props,
		&outImageBrush
	);

	ID2D1RectangleGeometry* clipRectGeo = nullptr;
	d2dFactory->CreateRectangleGeometry(clipRectD2D, &clipRectGeo);
	d2dDeviceContext->FillGeometry(clipRectGeo, outImageBrush);

	targetBitmap->Release();
	outImage->Release();
	outImageBrush->Release();
	clipRectGeo->Release();
}

std::wstring to_wide(const std::string& str) {
	return std::wstring(str.begin(), str.end());
}

uint64_t getTextLayoutKey(const std::string& textStr, float textSize) {
	std::hash<std::string> textHash;
	std::hash<float> textSizeHash;
	uint64_t combinedHash = textHash(textStr) ^ textSizeHash(textSize);
	return combinedHash;
}

IDWriteTextFormat* getTextFormat(float textSize, bool bold = false) {
	auto& cached = textFormatCache[textSize];
	if (!cached) {
		std::wstring fontNameWide = to_wide(currentD2DFont);
		const WCHAR* fontName = fontNameWide.c_str();
		d2dWriteFactory->CreateTextFormat(
			fontName,
			nullptr,
			bold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
			isFontItalic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			(float)currentD2DFontSize * textSize,
			L"en-us",
			cached.put()
		);
	}
	return cached.get();
}

IDWriteTextLayout* getTextLayout(const std::string& textStr, float textSize, bool storeTextLayout, bool bold) {
	std::wstring wideText = to_wide(textStr);
	const WCHAR* text = wideText.c_str();
	IDWriteTextFormat* textFormat = getTextFormat(textSize, bold);
	uint64_t textLayoutKey = getTextLayoutKey(textStr, textSize) ^ (bold ? 0xDEADBEEF : 0); // separate cache for bold

	if (storeTextLayout) {
		if (!textLayoutCache[textLayoutKey]) {
			d2dWriteFactory->CreateTextLayout(
				text,
				(UINT32)wcslen(text),
				textFormat,
				FLT_MAX,
				0.f,
				textLayoutCache[textLayoutKey].put()
			);
		}
		return textLayoutCache[textLayoutKey].get();
	}
	else {
		if (!textLayoutTemporary[textLayoutKey]) {
			d2dWriteFactory->CreateTextLayout(
				text,
				(UINT32)wcslen(text),
				textFormat,
				FLT_MAX,
				0.f,
				textLayoutTemporary[textLayoutKey].put()
			);
		}
		return textLayoutTemporary[textLayoutKey].get();
	}
}


ID2D1SolidColorBrush* getSolidColorBrush(const Color& color) {
	uint32_t colorBrushKey = Colors::ColorToUInt(color);
	if (colorBrushCache[colorBrushKey].get() == nullptr) {
		d2dDeviceContext->CreateSolidColorBrush(color.toD2D1Color(), colorBrushCache[colorBrushKey].put());
	}
	return colorBrushCache[colorBrushKey].get();
}

void D2D::fillRoundedRect(const Vector4<float>& rect, const Color& color, float radius) {
	if (!d2dDeviceContext || !d2dFactory) return;

	ID2D1SolidColorBrush* colorBrush = getSolidColorBrush(color);
	if (!colorBrush) return;

	ID2D1RoundedRectangleGeometry* roundedRectGeometry = nullptr;
	D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
		D2D1::RectF(rect.x, rect.y, rect.z, rect.w),
		radius,
		radius
	);

	HRESULT hr = d2dFactory->CreateRoundedRectangleGeometry(roundedRect, &roundedRectGeometry);
	if (FAILED(hr) || !roundedRectGeometry) return;

	d2dDeviceContext->FillGeometry(roundedRectGeometry, colorBrush);
	roundedRectGeometry->Release();
}

void D2D::drawRoundedRect(const Vector4<float>& rect, const Color& color, float radius, float width) {
	if (!d2dDeviceContext || !d2dFactory) return;

	ID2D1SolidColorBrush* colorBrush = getSolidColorBrush(color);
	if (!colorBrush) return;

	ID2D1RoundedRectangleGeometry* roundedRectGeometry = nullptr;
	D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
		D2D1::RectF(rect.x, rect.y, rect.z, rect.w),
		radius,
		radius
	);

	HRESULT hr = d2dFactory->CreateRoundedRectangleGeometry(roundedRect, &roundedRectGeometry);
	if (FAILED(hr) || !roundedRectGeometry) return;

	d2dDeviceContext->DrawGeometry(roundedRectGeometry, colorBrush, width);
	roundedRectGeometry->Release();
}

void D2D::beginClip(const Vector4<float>& clipRect) {
	if (!d2dDeviceContext) return;

	D2D1_RECT_F d2dClipRect = D2D1::RectF(clipRect.x, clipRect.y, clipRect.z, clipRect.w);

	ID2D1Layer* clipLayer = nullptr;
	HRESULT hr = d2dDeviceContext->CreateLayer(nullptr, &clipLayer);
	if (FAILED(hr) || !clipLayer) return;

	d2dDeviceContext->PushLayer(
		D2D1::LayerParameters(d2dClipRect),
		clipLayer
	);

	clipLayer->Release();
}

void D2D::endClip() {
	if (!d2dDeviceContext) return;
	d2dDeviceContext->PopLayer();
}

void D2D::fillTriangle(const Vector2<float>& p1, const Vector2<float>& p2, const Vector2<float>& p3, const Color& color) {
	ID2D1SolidColorBrush* colorBrush = getSolidColorBrush(color);

	ID2D1PathGeometry* triangleGeometry = nullptr;
	d2dFactory->CreatePathGeometry(&triangleGeometry);

	ID2D1GeometrySink* geometrySink = nullptr;
	triangleGeometry->Open(&geometrySink);

	geometrySink->BeginFigure(
		D2D1::Point2F(p1.x, p1.y),
		D2D1_FIGURE_BEGIN_FILLED
	);

	geometrySink->AddLine(D2D1::Point2F(p2.x, p2.y));
	geometrySink->AddLine(D2D1::Point2F(p3.x, p3.y));
	geometrySink->EndFigure(D2D1_FIGURE_END_CLOSED);

	geometrySink->Close();
	geometrySink->Release();

	d2dDeviceContext->FillGeometry(triangleGeometry, colorBrush);
	triangleGeometry->Release();
}

void D2D::drawTriangle(const Vector2<float>& p1, const Vector2<float>& p2, const Vector2<float>& p3, const Color& color, float width) {
	ID2D1SolidColorBrush* colorBrush = getSolidColorBrush(color);

	ID2D1PathGeometry* triangleGeometry = nullptr;
	d2dFactory->CreatePathGeometry(&triangleGeometry);

	ID2D1GeometrySink* geometrySink = nullptr;
	triangleGeometry->Open(&geometrySink);

	geometrySink->BeginFigure(
		D2D1::Point2F(p1.x, p1.y),
		D2D1_FIGURE_BEGIN_HOLLOW
	);

	geometrySink->AddLine(D2D1::Point2F(p2.x, p2.y));
	geometrySink->AddLine(D2D1::Point2F(p3.x, p3.y));
	geometrySink->EndFigure(D2D1_FIGURE_END_CLOSED);

	geometrySink->Close();
	geometrySink->Release();

	d2dDeviceContext->DrawGeometry(triangleGeometry, colorBrush, width);
	triangleGeometry->Release();
}