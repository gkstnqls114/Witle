#include "stdafx.h"
#include "GameScreen.h"
#include "MyRectangle.h"
#include "ShaderManager.h"
#include "PlayerStatus.h"

void PlayerStatus::ReleaseObjects()
{
	if (m_HpBar)
	{
		m_HpBar->ReleaseObjects();
		delete m_HpBar;
		m_HpBar = nullptr;
	}
}

void PlayerStatus::ReleaseUploadBuffers()
{
	if (m_HpBar) m_HpBar->ReleaseUploadBuffers();
}

PlayerStatus::PlayerStatus(GameObject * pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
	: ComponentBase(pOwner)
{
	m_HpBar = new MyRectangle(pOwner, pd3dDevice, pd3dCommandList, POINT{ int(GameScreen::GetWidth()) / 2, int(GameScreen::GetHeight()) - 70}, 300.f, 30.f, nullptr);
}

PlayerStatus::~PlayerStatus()
{
}

void PlayerStatus::Update(float fTimeElapsed)
{

}

void PlayerStatus::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{ 
	m_HpBar->Render(pd3dCommandList);
}
