#include "stdafx.h"
#include "GameScreen.h"
#include "MyRectangle.h"
#include "Player.h"
#include "ShaderManager.h"
#include "PlayerStatus.h"

void PlayerStatus::SetAnimationStateToHit()
{ 
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_HIT.ID);
}

void PlayerStatus::ReleaseObjects()
{
	if (m_HpBar)
	{
		m_HpBar->ReleaseObjects();
		delete m_HpBar;
		m_HpBar = nullptr;
	}

	if (m_MpBar)
	{
		m_MpBar->ReleaseObjects();
		delete m_MpBar;
		m_MpBar = nullptr;
	}
}

void PlayerStatus::ReleaseUploadBuffers()
{
	if (m_HpBar) m_HpBar->ReleaseUploadBuffers();
	if (m_MpBar) m_MpBar->ReleaseUploadBuffers();
}

PlayerStatus::PlayerStatus(GameObject * pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
	: Status(pOwner)
{
	m_pHPBarShader = ShaderManager::GetInstance()->GetShader(SHADER_SCREEN);
	m_pMPBarShader = ShaderManager::GetInstance()->GetShader(SHADER_SCREEN);

	m_HpBar = new MyRectangle(pOwner, pd3dDevice, pd3dCommandList, POINT{ int(GameScreen::GetWidth()) - 1100, int(GameScreen::GetHeight()) - 670}, 300.f, 30.f, nullptr);
	m_MpBar = new MyRectangle(pOwner, pd3dDevice, pd3dCommandList, POINT{ int(GameScreen::GetWidth()) - 1100, int(GameScreen::GetHeight()) - 620 }, 300.f, 30.f, nullptr);
}

PlayerStatus::~PlayerStatus()
{
}

void PlayerStatus::Update(float fTimeElapsed)
{

}

void PlayerStatus::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{ 
	float percentageHp = float(m_HP) / 1000.f * 100.f;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &percentageHp, 0);
	m_HpBar->Render(pd3dCommandList, m_pHPBarShader);

	float percentageMp = float(m_MP) / 1000.f * 100.f;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &percentageMp, 0);
	m_MpBar->Render(pd3dCommandList, m_pMPBarShader);
}
