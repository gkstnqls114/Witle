#include "stdafx.h"
#include "GameScreen.h"
#include "MyRectangle.h"
#include "PlayerStatus.h"

PlayerStatus::PlayerStatus(GameObject * pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
	: ComponentBase(pOwner)
{
	m_pHpBar = new MyRectangle(pOwner, pd3dDevice, pd3dCommandList, 
		POINT{static_cast<LONG>(GameScreen::GetWidth() / 2), static_cast<LONG>(GameScreen::GetHeight()) / 2 + 50}, 100.f, 30.f, 
		nullptr);
}

PlayerStatus::~PlayerStatus()
{
}

void PlayerStatus::Update(float fTimeElapsed)
{
	m_pHpBar->Update(fTimeElapsed);
}

void PlayerStatus::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_pHpBar->Render(pd3dCommandList);
}
