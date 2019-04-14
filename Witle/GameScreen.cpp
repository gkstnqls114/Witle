#include "stdafx.h"
#include "Camera.h"
#include "GameScreen.h"

UINT GameScreen::m_ClientWidth = 1280;
UINT GameScreen::m_ClientHeight = 720;

UINT GameScreen::m_Width = 1280;
UINT GameScreen::m_Height = 720;

UINT GameScreen::m_AnotherWidth = 1280 / 4;
UINT GameScreen::m_AnotherHeight = 720 / 4;
 
IDXGISwapChain* GameScreen::m_pSwapChain{ nullptr };

GameScreen::GameScreen()
{
}

GameScreen::~GameScreen()
{
}

bool GameScreen::ChangeWindowScreen(UINT width, UINT height)
{
	if (m_pSwapChain)
	{
		m_pSwapChain->SetFullscreenState(FALSE, NULL);
		DXGI_MODE_DESC dxgiTargetParameters;
		dxgiTargetParameters.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		dxgiTargetParameters.Width = width;
		dxgiTargetParameters.Height = height;
		dxgiTargetParameters.RefreshRate.Numerator = 60;
		dxgiTargetParameters.RefreshRate.Denominator = 1;
		dxgiTargetParameters.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		dxgiTargetParameters.ScanlineOrdering =
			DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		m_pSwapChain->ResizeTarget(&dxgiTargetParameters);

		m_Width = width;
		m_Height = height;

		return true;
	}
	else
	{
		return false;
	}
}

bool GameScreen::ChangeFullScreen(UINT width, UINT height)
{
	if (m_pSwapChain)
	{
		m_pSwapChain->SetFullscreenState(TRUE, NULL);
		DXGI_MODE_DESC dxgiTargetParameters;
		dxgiTargetParameters.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		dxgiTargetParameters.Width = width;
		dxgiTargetParameters.Height = height;
		dxgiTargetParameters.RefreshRate.Numerator = 60;
		dxgiTargetParameters.RefreshRate.Denominator = 1;
		dxgiTargetParameters.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		dxgiTargetParameters.ScanlineOrdering =
			DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		m_pSwapChain->ResizeTarget(&dxgiTargetParameters);
		
		m_Width = width;
		m_Height = height; 

		return true;
	}
	else
	{
		return false;
	}
}
