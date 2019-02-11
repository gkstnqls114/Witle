#include "stdafx.h"
#include "Camera.h"
#include "GameScreen.h"

UINT GameScreen::m_Width = 800;
UINT GameScreen::m_Height = 600;
Camera* GameScreen::m_pMainCamera{ nullptr };

GameScreen::GameScreen()
{
}

GameScreen::~GameScreen()
{
}

bool GameScreen::ChangeScreen(UINT width, UINT height)
{
	if (m_pMainCamera)
	{
		m_Width = width;
		m_Height = height;
		m_pMainCamera->SetViewport(0, 0, m_Width, m_Height);
		m_pMainCamera->SetScissorRect(0, 0, m_Width, m_Height);
		
		return true;
	}
	else
	{
		return false;
	}
}
