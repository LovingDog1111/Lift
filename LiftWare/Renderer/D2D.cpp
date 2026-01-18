#include "D2D.h"
#include <unordered_map>
#include <winrt/base.h>
#include "../Lift/Lift.h"
#include "../Lift/Features/FeatureFactory.h"
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
static ID2D1Bitmap1* shadowBitmap = nullptr;
static ID2D1Effect* shadowEffect = nullptr;
static ID2D1Effect* stencilEffect = nullptr;

static std::unordered_map<float, winrt::com_ptr<IDWriteTextFormat>> textFormatCache;
static std::unordered_map<uint64_t, winrt::com_ptr<IDWriteTextLayout>> textLayoutCache;
static std::unordered_map<uint32_t, winrt::com_ptr<ID2D1SolidColorBrush>> colorBrushCache;


static std::unordered_map<uint64_t, winrt::com_ptr<IDWriteTextLayout>> textLayoutTemporary;

static int currentD2DFontSize = 20;
static std::string currentD2DFont = "Arial";
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

		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(d2dWriteFactory),
			reinterpret_cast<IUnknown**>(&d2dWriteFactory));

		IDXGIDevice* dxgiDevice;
		d3d11Device->QueryInterface<IDXGIDevice>(&dxgiDevice);
		d2dFactory->CreateDevice(dxgiDevice, &d2dDevice);
		dxgiDevice->Release();

		d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dDeviceContext);
		// d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
		// &d2dDeviceContext);

		d2dDeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &blurEffect);
		blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
		blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION,
			D2D1_GAUSSIANBLUR_OPTIMIZATION_QUALITY);

		d2dDeviceContext->CreateEffect(CLSID_D2D1Shadow, &shadowEffect);

		d2dDeviceContext->CreateEffect(CLSID_D2D1AlphaMask, &stencilEffect);

		IDXGISurface* dxgiBackBuffer = nullptr;
		swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));
		D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), fxdpi, fxdpi);
		d2dDeviceContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer, &bitmapProperties,
			&sourceBitmap);
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

	SafeRelease(shadowBitmap);   // 新增的阴影位图
	SafeRelease(stencilEffect);  // 新增的模板效果
	SafeRelease(shadowEffect);   // 新增的阴影效果
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

	d2dDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

	ID2D1SolidColorBrush* shadowBrush = getSolidColorBrush(Color(0, 0, 0, color.a));
	d2dDeviceContext->DrawTextLayout(D2D1::Point2F(textPos.x + 1.f, textPos.y + 1.f), textLayout, shadowBrush);

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

	bool topLeft = roundType == CornerRoundType::TopOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Left || roundType == CornerRoundType::TopLeftOnly;
	bool topRight = roundType == CornerRoundType::TopOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Right;
	bool bottomLeft = roundType == CornerRoundType::BottomOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Left || roundType == CornerRoundType::BottomLeftOnly;
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

	bool topLeft = roundType == CornerRoundType::TopOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Left || roundType == CornerRoundType::TopLeftOnly;
	bool topRight = roundType == CornerRoundType::TopOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Right;
	bool bottomLeft = roundType == CornerRoundType::BottomOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Left || roundType == CornerRoundType::BottomLeftOnly;
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

void D2D::fillGradientRectangle(const Vector4<float>& rect, const Color& startColor, const Color& endColor, float rounding, CornerRoundType roundType) {
	D2D1_GRADIENT_STOP stops[2];
	stops[0].color = D2D1::ColorF(
		startColor.r / 255.0f,
		startColor.g / 255.0f,
		startColor.b / 255.0f,
		startColor.a / 255.0f
	);
	stops[0].position = 0.0f;

	stops[1].color = D2D1::ColorF(
		endColor.r / 255.0f,
		endColor.g / 255.0f,
		endColor.b / 255.0f,
		endColor.a / 255.0f
	);
	stops[1].position = 1.0f;

	ID2D1GradientStopCollection* gradientStops = nullptr;
	d2dDeviceContext->CreateGradientStopCollection(stops, 2, D2D1_GAMMA_1_0, D2D1_EXTEND_MODE_CLAMP, &gradientStops);

	D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES brushProps = {};
	brushProps.startPoint = D2D1::Point2F(rect.x, rect.y);
	brushProps.endPoint = D2D1::Point2F(rect.z, rect.w);

	ID2D1LinearGradientBrush* gradientBrush = nullptr;
	d2dDeviceContext->CreateLinearGradientBrush(brushProps, gradientStops, &gradientBrush);

	if (rounding <= 0.0f || roundType == CornerRoundType::None) {
		d2dDeviceContext->FillRectangle(D2D1::RectF(rect.x, rect.y, rect.z, rect.w), gradientBrush);
		gradientBrush->Release();
		gradientStops->Release();
		return;
	}

	if (roundType == CornerRoundType::Full) {
		D2D1_ROUNDED_RECT roundedRect = { D2D1::RectF(rect.x, rect.y, rect.z, rect.w), rounding, rounding };
		d2dDeviceContext->FillRoundedRectangle(&roundedRect, gradientBrush);
		gradientBrush->Release();
		gradientStops->Release();
		return;
	}

	ID2D1PathGeometry* geometry = nullptr;
	ID2D1GeometrySink* sink = nullptr;
	d2dFactory->CreatePathGeometry(&geometry);
	geometry->Open(&sink);

	float x1 = rect.x, y1 = rect.y, x2 = rect.z, y2 = rect.w;

	bool topLeft = roundType == CornerRoundType::TopOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Left || roundType == CornerRoundType::TopLeftOnly;
	bool topRight = roundType == CornerRoundType::TopOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Right;
	bool bottomLeft = roundType == CornerRoundType::BottomOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Left || roundType == CornerRoundType::BottomLeftOnly;
	bool bottomRight = roundType == CornerRoundType::BottomOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Right;

	sink->BeginFigure(D2D1::Point2F(x1 + (topLeft ? rounding : 0), y1), D2D1_FIGURE_BEGIN_FILLED);
	sink->AddLine(D2D1::Point2F(x2 - (topRight ? rounding : 0), y1));
	if (topRight) sink->AddArc(D2D1::ArcSegment(D2D1::Point2F(x2, y1 + rounding), D2D1::SizeF(rounding, rounding), 0.0f, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

	sink->AddLine(D2D1::Point2F(x2, y2 - (bottomRight ? rounding : 0)));
	if (bottomRight) sink->AddArc(D2D1::ArcSegment(D2D1::Point2F(x2 - rounding, y2), D2D1::SizeF(rounding, rounding), 0.0f, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

	sink->AddLine(D2D1::Point2F(x1 + (bottomLeft ? rounding : 0), y2));
	if (bottomLeft) sink->AddArc(D2D1::ArcSegment(D2D1::Point2F(x1, y2 - rounding), D2D1::SizeF(rounding, rounding), 0.0f, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

	sink->AddLine(D2D1::Point2F(x1, y1 + (topLeft ? rounding : 0)));
	if (topLeft) sink->AddArc(D2D1::ArcSegment(D2D1::Point2F(x1 + rounding, y1), D2D1::SizeF(rounding, rounding), 0.0f, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

	sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	sink->Close();
	sink->Release();

	d2dDeviceContext->FillGeometry(geometry, gradientBrush);
	geometry->Release();
	gradientBrush->Release();
	gradientStops->Release();
}

void D2D::fillGradientRectangleVertical(const Vector4<float>& rect, const Color& startColor, const Color& endColor, float rounding, CornerRoundType roundType) {
	D2D1_GRADIENT_STOP stops[2];
	stops[0].color = D2D1::ColorF(startColor.r / 255.0f, startColor.g / 255.0f, startColor.b / 255.0f, startColor.a / 255.0f);
	stops[0].position = 0.0f;
	stops[1].color = D2D1::ColorF(endColor.r / 255.0f, endColor.g / 255.0f, endColor.b / 255.0f, endColor.a / 255.0f);
	stops[1].position = 1.0f;

	ID2D1GradientStopCollection* gradientStops = nullptr;
	d2dDeviceContext->CreateGradientStopCollection(stops, 2, D2D1_GAMMA_1_0, D2D1_EXTEND_MODE_CLAMP, &gradientStops);

	D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES brushProps = {};
	brushProps.startPoint = D2D1::Point2F(rect.x, rect.y);
	brushProps.endPoint = D2D1::Point2F(rect.x, rect.w);

	ID2D1LinearGradientBrush* gradientBrush = nullptr;
	d2dDeviceContext->CreateLinearGradientBrush(brushProps, gradientStops, &gradientBrush);

	if (rounding <= 0.0f || roundType == CornerRoundType::None) {
		d2dDeviceContext->FillRectangle(D2D1::RectF(rect.x, rect.y, rect.z, rect.w), gradientBrush);
	}
	else if (roundType == CornerRoundType::Full) {
		D2D1_ROUNDED_RECT roundedRect = { D2D1::RectF(rect.x, rect.y, rect.z, rect.w), rounding, rounding };
		d2dDeviceContext->FillRoundedRectangle(&roundedRect, gradientBrush);
	}
	else {
		ID2D1PathGeometry* geometry = nullptr;
		ID2D1GeometrySink* sink = nullptr;
		d2dFactory->CreatePathGeometry(&geometry);
		geometry->Open(&sink);

		float x1 = rect.x, y1 = rect.y, x2 = rect.z, y2 = rect.w;
		bool topLeft = roundType == CornerRoundType::TopOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Left || roundType == CornerRoundType::TopLeftOnly;
		bool topRight = roundType == CornerRoundType::TopOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Right;
		bool bottomLeft = roundType == CornerRoundType::BottomOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Left || roundType == CornerRoundType::BottomLeftOnly;
		bool bottomRight = roundType == CornerRoundType::BottomOnly || roundType == CornerRoundType::SidesOnly || roundType == CornerRoundType::Right;

		sink->BeginFigure(D2D1::Point2F(x1 + (topLeft ? rounding : 0), y1), D2D1_FIGURE_BEGIN_FILLED);
		sink->AddLine(D2D1::Point2F(x2 - (topRight ? rounding : 0), y1));
		if (topRight) sink->AddArc(D2D1::ArcSegment(D2D1::Point2F(x2, y1 + rounding), D2D1::SizeF(rounding, rounding), 0.0f, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));
		sink->AddLine(D2D1::Point2F(x2, y2 - (bottomRight ? rounding : 0)));
		if (bottomRight) sink->AddArc(D2D1::ArcSegment(D2D1::Point2F(x2 - rounding, y2), D2D1::SizeF(rounding, rounding), 0.0f, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));
		sink->AddLine(D2D1::Point2F(x1 + (bottomLeft ? rounding : 0), y2));
		if (bottomLeft) sink->AddArc(D2D1::ArcSegment(D2D1::Point2F(x1, y2 - rounding), D2D1::SizeF(rounding, rounding), 0.0f, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));
		sink->AddLine(D2D1::Point2F(x1, y1 + (topLeft ? rounding : 0)));
		if (topLeft) sink->AddArc(D2D1::ArcSegment(D2D1::Point2F(x1 + rounding, y1), D2D1::SizeF(rounding, rounding), 0.0f, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

		sink->EndFigure(D2D1_FIGURE_END_CLOSED);
		sink->Close();
		sink->Release();

		d2dDeviceContext->FillGeometry(geometry, gradientBrush);
		geometry->Release();
	}

	gradientBrush->Release();
	gradientStops->Release();
}

void D2D::drawCircle(const Vector2<float>& centerPos, const Color& color, float radius, float width) {
	ID2D1SolidColorBrush* colorBrush = getSolidColorBrush(color);
	d2dDeviceContext->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(centerPos.x, centerPos.y), radius, radius), colorBrush, width);
}

void D2D::fillCircle(const Vector2<float>& centerPos, const Color& color, float radius) {
	ID2D1SolidColorBrush* colorBrush = getSolidColorBrush(color);
	d2dDeviceContext->FillEllipse(D2D1::Ellipse(D2D1::Point2F(centerPos.x, centerPos.y), radius, radius), colorBrush);
}

void D2D::drawGlowingCircle(const Vector2<float>& center, float radius, const Color& color, float glowRadius, float outlineThickness) {
	ID2D1SolidColorBrush* solidBrush = getSolidColorBrush(color);

	D2D1_RECT_F glowRect = D2D1::RectF(
		center.x - radius - glowRadius - outlineThickness / 2,
		center.y - radius - glowRadius - outlineThickness / 2,
		center.x + radius + glowRadius + outlineThickness / 2,
		center.y + radius + glowRadius + outlineThickness / 2
	);

	winrt::com_ptr<ID2D1BitmapRenderTarget> blurTarget;
	d2dDeviceContext->CreateCompatibleRenderTarget(
		D2D1::SizeF(glowRect.right - glowRect.left, glowRect.bottom - glowRect.top),
		blurTarget.put()
	);

	blurTarget->BeginDraw();
	blurTarget->Clear(D2D1::ColorF(0, 0, 0, 0));

	ID2D1SolidColorBrush* glowBrush;
	blurTarget->CreateSolidColorBrush(
		D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, 1.f),
		&glowBrush
	);

	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(radius + glowRadius, radius + glowRadius), radius, radius);
	blurTarget->DrawEllipse(ellipse, glowBrush, outlineThickness);
	blurTarget->EndDraw();

	winrt::com_ptr<ID2D1Bitmap> glowBitmap;
	blurTarget->GetBitmap(glowBitmap.put());

	winrt::com_ptr<ID2D1Effect> blurEffect;
	d2dDeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, blurEffect.put());
	blurEffect->SetInput(0, glowBitmap.get());
	blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, glowRadius / 2.f);
	blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);

	d2dDeviceContext->DrawImage(
		blurEffect.get(),
		D2D1::Point2F(glowRect.left, glowRect.top),
		D2D1::RectF(0, 0, glowRect.right - glowRect.left, glowRect.bottom - glowRect.top)
	);

	d2dDeviceContext->DrawEllipse(ellipse, solidBrush, outlineThickness);

	glowBrush->Release();
}


void D2D::fillGlowingCircle(const Vector2<float>& center, float radius, const Color& color, float glowRadius) {
	ID2D1SolidColorBrush* solidBrush = getSolidColorBrush(color);

	D2D1_RECT_F glowRect = D2D1::RectF(
		center.x - radius - glowRadius,
		center.y - radius - glowRadius,
		center.x + radius + glowRadius,
		center.y + radius + glowRadius
	);

	winrt::com_ptr<ID2D1BitmapRenderTarget> blurTarget;
	d2dDeviceContext->CreateCompatibleRenderTarget(
		D2D1::SizeF(glowRect.right - glowRect.left, glowRect.bottom - glowRect.top),
		blurTarget.put()
	);

	blurTarget->BeginDraw();
	blurTarget->Clear(D2D1::ColorF(0, 0, 0, 0));

	ID2D1SolidColorBrush* glowBrush;
	blurTarget->CreateSolidColorBrush(
		D2D1::ColorF(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f),
		&glowBrush
	);

	blurTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(radius + glowRadius, radius + glowRadius), radius, radius), glowBrush);
	blurTarget->EndDraw();

	winrt::com_ptr<ID2D1Bitmap> glowBitmap;
	blurTarget->GetBitmap(glowBitmap.put());

	winrt::com_ptr<ID2D1Effect> blurEffect;
	d2dDeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, blurEffect.put());
	blurEffect->SetInput(0, glowBitmap.get());
	blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, glowRadius / 2.0f);
	blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);

	d2dDeviceContext->DrawImage(
		blurEffect.get(),
		D2D1::Point2F(glowRect.left, glowRect.top),
		D2D1::RectF(0, 0, glowRect.right - glowRect.left, glowRect.bottom - glowRect.top)
	);

	d2dDeviceContext->FillEllipse(D2D1::Ellipse(D2D1::Point2F(center.x, center.y), radius, radius), solidBrush);

	glowBrush->Release();
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
		d2dDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
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
				FLT_MAX,
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
				FLT_MAX,
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

void D2D::fillBottomLeftRoundedRect(const Vector4<float>& rect, const Color& color, float radius) {
	if (!d2dDeviceContext || !d2dFactory) return;

	ID2D1SolidColorBrush* brush = getSolidColorBrush(color);
	if (!brush) return;

	ID2D1PathGeometry* path = nullptr;
	if (FAILED(d2dFactory->CreatePathGeometry(&path)) || !path) return;

	ID2D1GeometrySink* sink = nullptr;
	if (FAILED(path->Open(&sink)) || !sink) {
		path->Release();
		return;
	}

	sink->BeginFigure(D2D1::Point2F(rect.x, rect.y), D2D1_FIGURE_BEGIN_FILLED);

	sink->AddLine(D2D1::Point2F(rect.z, rect.y));
	sink->AddLine(D2D1::Point2F(rect.z, rect.w));
	sink->AddLine(D2D1::Point2F(rect.x + radius, rect.w));
	sink->AddArc(D2D1::ArcSegment(
		D2D1::Point2F(rect.x, rect.w - radius),
		D2D1::SizeF(radius, radius),
		0.f,
		D2D1_SWEEP_DIRECTION_CLOCKWISE,
		D2D1_ARC_SIZE_SMALL
	));
	sink->AddLine(D2D1::Point2F(rect.x, rect.y));

	sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	sink->Close();
	sink->Release();

	d2dDeviceContext->FillGeometry(path, brush);
	path->Release();
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

void D2D::drawNeuralNetworkParticlesOptimized(
	const Vector2<float>& screenSize,
	const Color& color,
	int particleCount,
	float particleRadius,
	float lineWidth,
	float maxDistance
) {
	if (!d2dDeviceContext || particleCount <= 0 || maxDistance <= 1.f ||
		screenSize.x <= 2.f || screenSize.y <= 2.f)
		return;

	static std::vector<Vector2<float>> p, v, t;
	static std::vector<int> degree;
	static int lastCount = 0;

	auto clamp01 = [](float x) { return (x < 0.f) ? 0.f : (x > 1.f ? 1.f : x); };

	auto initParticles = [&]() {
		lastCount = particleCount;
		p.assign(particleCount, {});
		v.assign(particleCount, {});
		t.assign(particleCount, {});
		degree.assign(particleCount, 0);

		int w = (int)std::max(1.f, screenSize.x);
		int h = (int)std::max(1.f, screenSize.y);

		for (int i = 0; i < particleCount; i++) {
			p[i].x = (float)(rand() % w);
			p[i].y = (float)(rand() % h);

			v[i].x = (float)(rand() % 100 - 50) / 600.0f;
			v[i].y = (float)(rand() % 100 - 50) / 600.0f;

			t[i].x = (float)(rand() % w);
			t[i].y = (float)(rand() % h);
		}
		};

	if (lastCount != particleCount || (int)p.size() != particleCount) {
		initParticles();
	}
	else {
		for (int i = 0; i < particleCount; i++) {
			p[i].x = std::max(0.f, std::min(p[i].x, screenSize.x));
			p[i].y = std::max(0.f, std::min(p[i].y, screenSize.y));
		}
		if ((int)degree.size() != particleCount) degree.assign(particleCount, 0);
	}

	ID2D1SolidColorBrush* brush = nullptr;
	d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1, 1), &brush);
	if (!brush) return;

	const float inv255 = 1.0f / 255.0f;
	const float r = color.r * inv255;
	const float g = color.g * inv255;
	const float b = color.b * inv255;
	const float baseA = color.a * inv255;

	const float dt = D2D::deltaTime * 60.f;

	for (int i = 0; i < particleCount; i++) {
		float dx = t[i].x - p[i].x;
		float dy = t[i].y - p[i].y;
		float d2 = dx * dx + dy * dy;

		if (d2 > 100.f) {
			float invD = 1.0f / std::sqrt(d2);
			const float accel = 0.012f;
			v[i].x += dx * invD * accel * dt;
			v[i].y += dy * invD * accel * dt;
		}
		else {
			int w = (int)std::max(1.f, screenSize.x);
			int h = (int)std::max(1.f, screenSize.y);
			t[i].x = (float)(rand() % w);
			t[i].y = (float)(rand() % h);
		}

		v[i].x *= 0.985f;
		v[i].y *= 0.985f;

		if ((rand() % 1000) < 10) {
			v[i].x += (float)(rand() % 100 - 50) / 4000.0f;
			v[i].y += (float)(rand() % 100 - 50) / 4000.0f;
		}

		float sp2 = v[i].x * v[i].x + v[i].y * v[i].y;
		const float maxSp = 0.7f;
		if (sp2 > maxSp * maxSp) {
			float invSp = 1.0f / std::sqrt(sp2);
			v[i].x = v[i].x * invSp * maxSp;
			v[i].y = v[i].y * invSp * maxSp;
		}

		p[i].x += v[i].x * dt;
		p[i].y += v[i].y * dt;

		if (p[i].x < 0.f) { p[i].x = 0.f;          v[i].x *= -0.7f; }
		if (p[i].x > screenSize.x) { p[i].x = screenSize.x; v[i].x *= -0.7f; }
		if (p[i].y < 0.f) { p[i].y = 0.f;          v[i].y *= -0.7f; }
		if (p[i].y > screenSize.y) { p[i].y = screenSize.y; v[i].y *= -0.7f; }
	}

	const float cellSize = std::max(8.f, maxDistance);
	const int gridW = (int)(screenSize.x / cellSize) + 1;
	const int gridH = (int)(screenSize.y / cellSize) + 1;
	const int cellCount = gridW * gridH;

	static std::vector<int> head;
	static std::vector<int> next;
	head.assign(cellCount, -1);
	next.assign(particleCount, -1);

	auto cellIndex = [&](float x, float y) -> int {
		int cx = (int)(x / cellSize);
		int cy = (int)(y / cellSize);
		if (cx < 0) cx = 0; else if (cx >= gridW) cx = gridW - 1;
		if (cy < 0) cy = 0; else if (cy >= gridH) cy = gridH - 1;
		return cy * gridW + cx;
		};

	for (int i = 0; i < particleCount; i++) {
		int c = cellIndex(p[i].x, p[i].y);
		next[i] = head[c];
		head[c] = i;
	}

	struct Seg { float x1, y1, x2, y2; };
	constexpr int ALPHA_STEPS = 12;
	static std::vector<Seg> buckets[ALPHA_STEPS];
	for (int s = 0; s < ALPHA_STEPS; s++) buckets[s].clear();

	std::fill(degree.begin(), degree.end(), 0);

	const float maxD2 = maxDistance * maxDistance;
	const float baseLineAlpha = 0.60f;

	const int off[9][2] = {
		{-1,-1},{0,-1},{1,-1},
		{-1, 0},{0, 0},{1, 0},
		{-1, 1},{0, 1},{1, 1}
	};

	for (int i = 0; i < particleCount; i++) {
		int cx = (int)(p[i].x / cellSize);
		int cy = (int)(p[i].y / cellSize);
		if (cx < 0) cx = 0; else if (cx >= gridW) cx = gridW - 1;
		if (cy < 0) cy = 0; else if (cy >= gridH) cy = gridH - 1;

		for (int k = 0; k < 9; k++) {
			int nx = cx + off[k][0];
			int ny = cy + off[k][1];
			if (nx < 0 || nx >= gridW || ny < 0 || ny >= gridH) continue;

			int c = ny * gridW + nx;
			for (int j = head[c]; j != -1; j = next[j]) {
				if (j <= i) continue;

				float dx = p[i].x - p[j].x;
				float dy = p[i].y - p[j].y;
				float d2 = dx * dx + dy * dy;
				if (d2 >= maxD2) continue;

				float d = std::sqrt(d2);
				float a = (1.0f - (d / maxDistance)) * baseLineAlpha;
				if (a <= 0.f) continue;

				degree[i]++; degree[j]++;

				int step = (int)(a * (ALPHA_STEPS - 1) / baseLineAlpha);
				if (step < 0) step = 0;
				if (step >= ALPHA_STEPS) step = ALPHA_STEPS - 1;

				buckets[step].push_back({ p[i].x, p[i].y, p[j].x, p[j].y });
			}
		}
	}

	for (int step = 0; step < ALPHA_STEPS; step++) {
		if (buckets[step].empty()) continue;

		float a01 = (baseLineAlpha * (float)step / (float)(ALPHA_STEPS - 1));
		brush->SetColor(D2D1::ColorF(r, g, b, a01 * baseA));

		for (const auto& s : buckets[step]) {
			d2dDeviceContext->DrawLine(
				D2D1::Point2F(s.x1, s.y1),
				D2D1::Point2F(s.x2, s.y2),
				brush,
				lineWidth
			);
		}
	}

	for (int i = 0; i < particleCount; i++) {
		float sp = std::sqrt(v[i].x * v[i].x + v[i].y * v[i].y);
		float connN = clamp01((float)degree[i] / 10.f);
		float spN = clamp01(sp / 0.7f);
		float intensity = clamp01(0.35f + 0.45f * connN + 0.20f * spN);

		float glowR = particleRadius * (2.6f + 1.0f * connN);
		float haloR = particleRadius * (1.6f + 0.5f * connN);
		float coreR = particleRadius * (0.85f + 0.35f * intensity);

		// body
		brush->SetColor(D2D1::ColorF(r, g, b, baseA * (0.30f + 0.25f * intensity)));
		d2dDeviceContext->FillEllipse(
			D2D1::Ellipse(D2D1::Point2F(p[i].x, p[i].y), particleRadius * 1.05f, particleRadius * 1.05f),
			brush
		);

		// bright core
		brush->SetColor(D2D1::ColorF(r, g, b, baseA * (0.18f + 0.35f * intensity)));
		d2dDeviceContext->FillEllipse(
			D2D1::Ellipse(D2D1::Point2F(p[i].x, p[i].y), coreR, coreR),
			brush
		);

		float vx = v[i].x, vy = v[i].y;
		float vl = std::sqrt(vx * vx + vy * vy);
		if (vl > 0.0001f) {
			vx /= vl; vy /= vl;
			float px = -vy, py = vx;

			float spikeLen = particleRadius * (2.2f + 2.0f * spN + 1.5f * connN);
			float spikeOff = particleRadius * (1.2f + 0.6f * connN);
			float side = particleRadius * (0.9f + 0.6f * spN);

			brush->SetColor(D2D1::ColorF(r, g, b, baseA * (0.18f + 0.25f * intensity)));

			d2dDeviceContext->DrawLine(
				D2D1::Point2F(p[i].x + vx * spikeOff, p[i].y + vy * spikeOff),
				D2D1::Point2F(p[i].x + vx * (spikeOff + spikeLen), p[i].y + vy * (spikeOff + spikeLen)),
				brush,
				std::max(0.75f, lineWidth * 0.75f)
			);

			d2dDeviceContext->DrawLine(
				D2D1::Point2F(p[i].x + vx * spikeOff + px * side, p[i].y + vy * spikeOff + py * side),
				D2D1::Point2F(p[i].x + vx * (spikeOff + spikeLen * 0.65f) + px * side * 0.4f,
					p[i].y + vy * (spikeOff + spikeLen * 0.65f) + py * side * 0.4f),
				brush,
				std::max(0.65f, lineWidth * 0.65f)
			);

			d2dDeviceContext->DrawLine(
				D2D1::Point2F(p[i].x + vx * spikeOff - px * side, p[i].y + vy * spikeOff - py * side),
				D2D1::Point2F(p[i].x + vx * (spikeOff + spikeLen * 0.65f) - px * side * 0.4f,
					p[i].y + vy * (spikeOff + spikeLen * 0.65f) - py * side * 0.4f),
				brush,
				std::max(0.65f, lineWidth * 0.65f)
			);
		}
	}

	brush->Release();
}

void D2D::drawStarParticles(const Vector2<float>& screenSize, const Color& color, int particleCount, float speed, float size) {
	static std::vector<Vector2<float>> particles;
	static std::vector<float> sizes;
	static std::vector<float> opacities;
	static std::vector<float> angles;
	static std::vector<float> rotationSpeeds;
	static bool initialized = false;

	if (!initialized) {
		particles.resize(particleCount);
		sizes.resize(particleCount);
		opacities.resize(particleCount);
		angles.resize(particleCount);
		rotationSpeeds.resize(particleCount);

		for (int i = 0; i < particleCount; i++) {
			particles[i].x = (float)(rand() % (int)screenSize.x);
			particles[i].y = (float)(rand() % (int)screenSize.y);
			sizes[i] = (float)(rand() % (int)(size * 100)) / 100.0f + 0.5f;
			opacities[i] = (float)(rand() % 70 + 30) / 100.0f; 
			angles[i] = (float)(rand() % 360);
			rotationSpeeds[i] = (float)(rand() % 20 - 10) / 10.0f;
		}
		initialized = true;
	}

	for (int i = 0; i < particleCount; i++) {
		angles[i] += rotationSpeeds[i];
		if (angles[i] > 360.0f) angles[i] -= 360.0f;
		if (angles[i] < 0.0f) angles[i] += 360.0f;

		if (rand() % 100 < 5) { 
			particles[i].x += (float)(rand() % 5 - 2) * speed;
			particles[i].y += (float)(rand() % 5 - 2) * speed;

			particles[i].x = std::max(0.0f, std::min(particles[i].x, screenSize.x));
			particles[i].y = std::max(0.0f, std::min(particles[i].y, screenSize.y));
		}

		if (rand() % 100 < 10) { 
			opacities[i] = (float)(rand() % 70 + 30) / 100.0f;
		}

		Color starColor(color.r, color.g, color.b, (uint8_t)(color.a * opacities[i]));
		ID2D1SolidColorBrush* starBrush = getSolidColorBrush(starColor);

		float halfSize = sizes[i] / 2.0f;
		float centerX = particles[i].x;
		float centerY = particles[i].y;

		float rad = angles[i] * (3.14159265f / 180.0f);
		float cosA = cosf(rad);
		float sinA = sinf(rad);

		D2D1_POINT_2F points[8];
		for (int j = 0; j < 4; j++) {
			float angle = (float)j * (3.14159265f / 2.0f) + rad;
			float x1 = centerX + cosf(angle) * halfSize;
			float y1 = centerY + sinf(angle) * halfSize;
			float x2 = centerX + cosf(angle) * (halfSize * 0.5f);
			float y2 = centerY + sinf(angle) * (halfSize * 0.5f);

			points[j * 2] = D2D1::Point2F(x1, y1);
			points[j * 2 + 1] = D2D1::Point2F(x2, y2);
		}

		d2dDeviceContext->DrawLine(points[0], points[1], starBrush, 1.0f);
		d2dDeviceContext->DrawLine(points[1], points[2], starBrush, 1.0f);
		d2dDeviceContext->DrawLine(points[2], points[3], starBrush, 1.0f);
		d2dDeviceContext->DrawLine(points[3], points[4], starBrush, 1.0f);
		d2dDeviceContext->DrawLine(points[4], points[5], starBrush, 1.0f);
		d2dDeviceContext->DrawLine(points[5], points[6], starBrush, 1.0f);
		d2dDeviceContext->DrawLine(points[6], points[7], starBrush, 1.0f);
		d2dDeviceContext->DrawLine(points[7], points[0], starBrush, 1.0f);
	}
}

#define D2D_CTX_GUARD()   \
    if(!d2dDeviceContext) \
        return;
#define D2D_CTX_GUARD_RET(defaultVal) \
    if(!d2dDeviceContext)             \
        return defaultVal;

void D2D::addShadow(const Vector4<float>& rect, float strength, const Color& shadowColor,
	float rounding, const std::vector<Vector4<float>>& excludeRects) {
	D2D_CTX_GUARD();

	if (!d2dDeviceContext)
		return;

	Vector2<float> windowSize = getWindowSize();

	// 创建阴影位图（如果还没有）
	if (shadowBitmap == nullptr) {
		D2D1_BITMAP_PROPERTIES1 newLayerProps =
			D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET, sourceBitmap->GetPixelFormat());
		d2dDeviceContext->CreateBitmap(sourceBitmap->GetPixelSize(), nullptr, 0, newLayerProps,
			&shadowBitmap);
	}

	// 在阴影位图上绘制形状
	d2dDeviceContext->SetTarget(shadowBitmap);
	d2dDeviceContext->Clear(D2D1::ColorF(0, 0, 0, 0));

	// 创建颜色画刷
	ID2D1SolidColorBrush* colorBrush =
		getSolidColorBrush(Color(shadowColor.r, shadowColor.g, shadowColor.b, 255));

	// 绘制圆角矩形
	D2D1_ROUNDED_RECT roundedRect =
		D2D1::RoundedRect(D2D1::RectF(rect.x, rect.y, rect.z, rect.w), rounding, rounding);
	d2dDeviceContext->FillRoundedRectangle(roundedRect, colorBrush);

	// 应用高斯模糊
	blurEffect->SetInput(0, shadowBitmap);
	blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, strength);
	blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
	blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION,
		D2D1_GAUSSIANBLUR_OPTIMIZATION_QUALITY);

	// 如果有排除区域，创建遮罩
	ID2D1Bitmap1* holeMaskBitmap = nullptr;
	if (!excludeRects.empty()) {
		D2D1_BITMAP_PROPERTIES1 maskProps =
			D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET, sourceBitmap->GetPixelFormat());
		d2dDeviceContext->CreateBitmap(sourceBitmap->GetPixelSize(), nullptr, 0, maskProps,
			&holeMaskBitmap);

		d2dDeviceContext->SetTarget(holeMaskBitmap);
		d2dDeviceContext->Clear(D2D1::ColorF(1, 1, 1, 1));  // 白色背景

		// 创建透明画刷来"挖洞"
		ID2D1SolidColorBrush* transparentBrush = nullptr;
		d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f),
			&transparentBrush);

		// 保存原始混合模式
		D2D1_PRIMITIVE_BLEND originalBlend = d2dDeviceContext->GetPrimitiveBlend();
		d2dDeviceContext->SetPrimitiveBlend(D2D1_PRIMITIVE_BLEND_COPY);

		// 在排除区域绘制透明矩形
		for (const auto& excludeRect : excludeRects) {
			D2D1_RECT_F rectF =
				D2D1::RectF(excludeRect.x, excludeRect.y, excludeRect.z, excludeRect.w);
			ID2D1RectangleGeometry* rectGeo = nullptr;
			d2dFactory->CreateRectangleGeometry(rectF, &rectGeo);
			d2dDeviceContext->FillGeometry(rectGeo, transparentBrush);
			rectGeo->Release();
		}

		// 恢复混合模式
		d2dDeviceContext->SetPrimitiveBlend(originalBlend);
		transparentBrush->Release();
	}

	// 切换回源位图
	d2dDeviceContext->SetTarget(sourceBitmap);

	// 获取模糊输出
	ID2D1Image* blurOutput = nullptr;
	blurEffect->GetOutput(&blurOutput);

	ID2D1Image* finalOutput = blurOutput;

	// 如果有遮罩，应用 Alpha 遮罩效果
	if (holeMaskBitmap) {
		stencilEffect->SetInput(0, blurOutput);
		stencilEffect->SetInput(1, holeMaskBitmap);
		stencilEffect->GetOutput(&finalOutput);
	}

	// 创建图像画刷
	ID2D1ImageBrush* imageBrush = nullptr;
	D2D1_IMAGE_BRUSH_PROPERTIES brushProps =
		D2D1::ImageBrushProperties(D2D1::RectF(0, 0, windowSize.x, windowSize.y));
	d2dDeviceContext->CreateImageBrush(finalOutput, brushProps, &imageBrush);

	// 设置画刷透明度
	imageBrush->SetOpacity(shadowColor.a / 255.0f);

	// 绘制到整个屏幕
	ID2D1RectangleGeometry* screenGeo = nullptr;
	d2dFactory->CreateRectangleGeometry(D2D1::RectF(0, 0, windowSize.x, windowSize.y), &screenGeo);
	d2dDeviceContext->FillGeometry(screenGeo, imageBrush);

	// 清理资源
	screenGeo->Release();
	imageBrush->Release();
	blurOutput->Release();

	if (holeMaskBitmap) {
		if (finalOutput != blurOutput) {
			finalOutput->Release();
		}
		holeMaskBitmap->Release();
	}
}

void D2D::addDropShadow(const Vector4<float>& rect, float blurRadius,
	const Color& shadowColor, const Vector2<float>& offset,
	float rounding) {
	D2D_CTX_GUARD();
	Vector4<float> shadowRect =
		Vector4<float>(rect.x + offset.x, rect.y + offset.y, rect.z + offset.x, rect.w + offset.y);
	std::vector<Vector4<float>> excludeRects = { rect };
	addShadow(shadowRect, blurRadius, shadowColor, rounding, excludeRects);
}