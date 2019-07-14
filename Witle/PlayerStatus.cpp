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
	if (m_GuageBar)
	{
		m_GuageBar->ReleaseObjects();
		delete m_GuageBar;
		m_GuageBar = nullptr;
	} 
}

void PlayerStatus::ReleaseUploadBuffers()
{
	if (m_GuageBar) m_GuageBar->ReleaseUploadBuffers(); 
}

PlayerStatus::PlayerStatus(GameObject * pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
	POINT center, float width, float height, const wchar_t * filepath)
	: Status(pOwner)
{
	m_pGuageBarShader = ShaderManager::GetInstance()->GetShader(SHADER_SCREEN); 
	m_GuageBar = new MyRectangle(pOwner, pd3dDevice, pd3dCommandList, center, width, height, filepath);
}

PlayerStatus::~PlayerStatus()
{
}

void PlayerStatus::Update(float fTimeElapsed)
{

}

void PlayerStatus::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{ 
	float percentageHp = float(m_Guage) / float(m_MAXGuage) * 100.f;
	pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_HPPERCENTAGE, 1, &percentageHp, 0);
	m_GuageBar->Render(pd3dCommandList, m_pGuageBarShader); 
}
