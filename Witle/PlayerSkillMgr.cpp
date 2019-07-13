#include "stdafx.h"

// Skill Effect 관련 ////////////////////////////
#include "SkillEffect.h"
#include "FireBall.h"
#include "IceBall.h"
#include "ElectricBall.h"
#include "PlayerManager.h"
// Skill Effect 관련 ////////////////////////////

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

}

void PlayerSkillMgr::Update(float fElapsedTime)
{
	Deactivate();

	for (int x = 0; x < m_count; ++x)
	{
		if (m_skill[x].isActive)
		{
			m_skill[x].skillEffect->Update(fElapsedTime);

		}
	}
}

void PlayerSkillMgr::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	for (int x = 0; x < m_count; ++x)
	{
		if (!m_skill[x].isActive) continue;

		XMFLOAT4X4 xmf4x4World;
		XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_skill[x].skillEffect->GetTransform().GetWorldMatrix()))); 
		pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &xmf4x4World, 0);
		m_skill[x].skillEffect->Render(pd3dCommandList, isGBuffers);
	}
}

void PlayerSkillMgr::Activate()
{
	for (int x = 0; x < m_count; ++x)
	{
		if (m_skill[x].isActive) continue;
		m_skill[x].isActive = true;
		 
		m_skill[x].spawnPosition =
			PlayerManager::GetMainPlayer()->GetTransform().GetPosition();

		m_skill[x].skillEffect->SetVelocity(
			m_skill[x].spawnPosition,
			75,
			PlayerManager::GetMainPlayer()->GetTransform().GetLook()
		);

		break;
	}
}

void PlayerSkillMgr::Deactive(int index)
{ 
	m_skill[index].isActive = false;
}

void PlayerSkillMgr::Deactivate()
{
	for (int x = 0; x < m_count; ++x)
	{
		if (!m_skill[x].isActive) continue;

		if (Vector3::Length(m_skill[x].spawnPosition, m_skill[x].skillEffect->GetTransform().GetPosition()) > m_distance)
		{
			m_skill[x].isActive = false;
		}
	}
}
 