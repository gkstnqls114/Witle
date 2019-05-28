#include "stdafx.h"
#include "GameScreen.h"
#include "MyRectangle.h"
#include "ShaderManager.h"
#include "PlayerStatus.h"

void PlayerStatus::SetAnimationState(int state)
{
	
}

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
	: Status(pOwner)
{
	m_pHPBarShader = ShaderManager::GetInstance()->GetShader(SHADER_SCREEN);

	m_HpBar = new MyRectangle(pOwner, pd3dDevice, pd3dCommandList, POINT{ int(GameScreen::GetWidth()) - 200, int(GameScreen::GetHeight()) - 70}, 300.f, 30.f, nullptr);
}

PlayerStatus::~PlayerStatus()
{
}

void PlayerStatus::Update(float fTimeElapsed)
{

}

void PlayerStatus::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{ 
	float percentage = float(m_HP) / 1000.f * 100.f;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &percentage, 0);
	m_HpBar->Render(pd3dCommandList, m_pHPBarShader);
}
