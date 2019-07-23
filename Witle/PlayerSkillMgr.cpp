#include "stdafx.h"

// Skill Effect 관련 ////////////////////////////
#include "SkillEffect.h"
#include "FireBall.h"
#include "IceBall.h"
#include "LightingBall.h"
#include "PlayerManager.h"
// Skill Effect 관련 ////////////////////////////

#include "PlayerStatus.h"
#include "Player.h"
#include "PlayerSkillMgr.h"

PlayerSkillMgr::PlayerSkillMgr(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_skill = new SKILL[m_count];

	m_skill[0].skillEffect = new FireBall("FireBall", pd3dDevice, pd3dCommandList);
	m_skill[1].skillEffect = new IceBall("FireBall", pd3dDevice, pd3dCommandList);
	m_skill[2].skillEffect = new LightingBall("FireBall", pd3dDevice, pd3dCommandList);
	m_skill[3].skillEffect = new FireBall("FireBall", pd3dDevice, pd3dCommandList); 
}

PlayerSkillMgr::~PlayerSkillMgr()
{
}

void PlayerSkillMgr::UpdatePhysics(float fElapsedTime)
{  

}

void PlayerSkillMgr::Update(float fElapsedTime)
{
	for (int x = 0; x < m_count; ++x)
	{
		if (m_skill[x].RemainCoolTime <= 0.f)
		{
			m_skill[x].RemainCoolTime = 0.f;
		}
		else
		{
			m_skill[x].RemainCoolTime -= fElapsedTime;
			m_skill[x].RemainCoolTimePrecentage = m_skill[x].RemainCoolTime / m_skill[x].skillEffect->m_CoolTime;
		}
	}

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

void PlayerSkillMgr::Activate(PlayerStatus* MPstaus, ENUM_SKILL skilltype)
{ 
	if (m_skill[skilltype].isActive) return;
	if (m_skill[skilltype].RemainCoolTime > 0.f) return;
	if ((MPstaus->m_Guage - 10/*사용하는 마나 게이지*/) < 0.f) return;

	MPstaus->m_Guage -= 10.f;
	m_skill[skilltype].isActive = true;
	m_skill[skilltype].RemainCoolTime = m_skill[skilltype].skillEffect->m_CoolTime;
	m_skill[skilltype].spawnPosition =
		PlayerManager::GetMainPlayer()->GetTransform().GetPosition();

	m_skill[skilltype].skillEffect->SetVelocity(
		m_skill[skilltype].spawnPosition,
		75,
		PlayerManager::GetMainPlayer()->GetTransform().GetLook()
	); 
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
 