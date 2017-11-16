#include "pch.h"
#include "MapRenderer.h"

#include "..\Common\DirectXHelper.h"
#include "Common\utility.h"

using namespace mapworld;

using namespace DirectX;
using namespace Windows::Foundation;

MapRenderer::MapRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	DX::ThrowIfFailed(
		m_deviceResources->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock)
		);
	CreateDeviceDependentResources();
}

void MapRenderer::Update()
{
	ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();
	Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();

	// reset iterator to beginning of color vector
	m_it = m_tileColors.begin();
}

void MapRenderer::Render()
{
	ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();
	Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();

	context->SaveDrawingState(m_stateBlock.Get());
	context->BeginDraw();

	INT rowNumber = 0;
	INT columnNumber = 0;
	for (FLOAT y = m_topBottomEdges; rowNumber < m_numberOfRows; y += m_side)
	{
		for (FLOAT x = m_leftRightEdges; columnNumber < m_numberOfColumns; x += m_side)
		{
			DrawTile(x, y, m_side, GetNextColor());
			columnNumber++;
		}
		rowNumber++;
		columnNumber = 0;
	}

	// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr = context->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}

	context->RestoreDrawingState(m_stateBlock.Get());
}

void MapRenderer::CreateDeviceDependentResources()
{
	Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();
	m_numberOfColumns = 40;
	m_numberOfRows = 22;
	m_leftRightEdges = 40;
	m_topBottomEdges = 32;
	FLOAT modifiedWidth = logicalSize.Width - (m_leftRightEdges * 2);
//	m_side = modifiedWidth / m_numberOfColumns;
	m_side = 38.0f;

	Trace(L"logical size width %f\n", logicalSize.Width);
	Trace(L"logical size height %f\n", logicalSize.Height);
	Trace(L"columns %d\n", m_numberOfColumns);
	Trace(L"rows %d\n", m_numberOfRows);
	Trace(L"side %f\n", m_side);
	Trace(L"left right edges %f\n", m_leftRightEdges);
	Trace(L"top bottom edges %f\n", m_topBottomEdges);

	DX::ThrowIfFailed(
		m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_solidColorBrush)
		);

	srand(time(nullptr));	// seed random num generator
	for (int i = 0; i < m_numberOfRows*m_numberOfColumns; i++)
	{
		FLOAT r;
		FLOAT g;
		FLOAT b;
		r = (float)(rand() % 255);
		g = (float)(rand() % 255);
		b = (float)(rand() % 255);
		m_tileColors.push_back(D2D1::ColorF(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f));
	}
//	m_tileColors.push_back(D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));
}
void MapRenderer::ReleaseDeviceDependentResources()
{
	m_solidColorBrush.Reset();
}

void MapRenderer::DrawTile(FLOAT x, FLOAT y, FLOAT side, FLOAT r, FLOAT g, FLOAT b)
{
	D2D1::ColorF color = D2D1::ColorF(r, g, b, 1.0f);
	m_solidColorBrush->SetColor(color);
	m_deviceResources->GetD2DDeviceContext()->FillRectangle(D2D1::RectF(x, y, x + side, y + side), m_solidColorBrush.Get());
}

void MapRenderer::DrawTile(FLOAT x, FLOAT y, FLOAT side, D2D1::ColorF color)
{
	m_solidColorBrush->SetColor(color);
	m_deviceResources->GetD2DDeviceContext()->FillRectangle(D2D1::RectF(x, y, x + side, y + side), m_solidColorBrush.Get());
}

D2D1::ColorF MapRenderer::GetNextColor()
{
	D2D1::ColorF retval = D2D1::ColorF::White;
	if (m_it != m_tileColors.end())
	{
		retval = *m_it;
		++m_it;
	}
	else
	{
		m_it = m_tileColors.begin();
		retval = GetNextColor();
	}
	return retval;
}
