#include "stdafx.h"
#include "GameScreen.h"
#include "MyRectangle.h"
#include "Monster.h"
#include "ShaderManager.h"
#include "MonsterStatus.h"

void MonsterStatus::SetAnimationStateToHit()
{ 
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_HIT.ID);
}

void MonsterStatus::ReleaseObjects()
{
	if (m_HpBar)
	{
		m_HpBar->ReleaseObjects();
		delete m_HpBar;
		m_HpBar = nullptr;
	}
}

void MonsterStatus::ReleaseUploadBuffers()
{
	if (m_HpBar) m_HpBar->ReleaseUploadBuffers();
}

MonsterStatus::MonsterStatus(GameObject * pOwner, ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
	: Status(pOwner)
{

}

MonsterStatus::~MonsterStatus()
{
}

void MonsterStatus::Update(float fTimeElapsed)
{

}
 
void MonsterStatus::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{

}
