#include "stdafx.h"

// Skill Effect ���� ////////////////////////////
#include "SkillEffect.h"
#include "FireBall.h"
#include "IceBall.h"
#include "ElectricBall.h"
#include "PlayerManager.h"
// Skill Effect ���� ////////////////////////////

#include "Player.h"
#include "PlayerSkillMgr.h"

PlayerSkillMgr::PlayerSkillMgr(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_skill = new SKILL[m_count];

	for (int x = 0; x < m_count; ++x)
	{
		m_skill[x].skillEffect = new FireBall("FireBall", pd3dDevice, pd3dCommandList);
	}
}

PlayerSkillMgr::~PlayerSkillMgr()
{
}

void PlayerSkillMgr::UpdatePhysics(float fElapsedTime)
{ 
	for (int x = 0; x < m_count; ++x)
	{
		if (!m_skill[x].isActive) continue;
		
		// ���� Ȱ��ȭ�� �Ǿ��ְ� �������� �ʴ� ����
		if (!m_skill[x].isMoving)
		{
			m_skill[x].isMoving = true;
			m_skill[x].skillEffect->SetVelocity(
				PlayerManager::GetMainPlayer()->GetTransform().GetPosition(),
				PlayerManager::GetMainPlayer()->GetTransform().GetLook()
			);
		} 
	}

}

void PlayerSkillMgr::Update(float fElapsedTime)
{
	for (int x = 0; x < m_count; ++x)
	{
		if (!m_skill[x].isMoving) continue;
		m_skill[x].skillEffect->Update(fElapsedTime);
	}
}

void PlayerSkillMgr::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	for (int x = 0; x < m_count; ++x)
	{
		if (!m_skill[x].isActive) continue;
		pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 4, &m_skill[x].skillEffect->GetTransform().GetWorldMatrix(), 0);
		m_skill[x].skillEffect->Render(pd3dCommandList, isGBuffers);
	}
}

void PlayerSkillMgr::Activate()
{
	for (int x = 0; x < m_count; ++x)
	{
		if (m_skill[x].isActive) continue;
		m_skill[x].isActive = true;
		break;
	}
}
 