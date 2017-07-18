#pragma once

#include "..\Common\DeviceResources.h"
#include <vector>

namespace mapworld
{
	class MapRenderer
	{
	public:
		MapRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();
		void Update();
		void Render();
	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock>  m_stateBlock;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_solidColorBrush;
		std::vector<D2D1::ColorF>						m_tileColors;
		std::vector<D2D1::ColorF>::iterator				m_it;
		INT	m_numberOfColumns;
		INT m_numberOfRows;
		FLOAT m_side;
		FLOAT m_leftRightEdges;
		FLOAT m_topBottomEdges;

		void MapRenderer::DrawTile(FLOAT x, FLOAT y, FLOAT side, FLOAT r, FLOAT g, FLOAT b);
		void MapRenderer::DrawTile(FLOAT x, FLOAT y, FLOAT side, D2D1::ColorF color);
		D2D1::ColorF MapRenderer::GetNextColor();
		void MapRenderer::DrawDot(FLOAT x, FLOAT y, FLOAT r, FLOAT g, FLOAT b);
	};
}

