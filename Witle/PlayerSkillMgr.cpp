#include "stdafx.h"

// Skill Effect 관련 ////////////////////////////
#include "SkillEffect.h"
#include "FireBall.h"
#include "IceBall.h"
#include "LightningBall.h"
#include "PlayerManager.h"
// Skill Effect 관련 ////////////////////////////

#include "Skill.h"
#include "PlayerStatus.h"
#include "Player.h"
#include "PlayerSkillMgr.h"

PlayerSkillMgr* PlayerSkillMgr::m_Instance{ nullptr };

PlayerSkillMgr::PlayerSkillMgr()
{
	m_skill = new SelectableSkill[SKILL_SELECTED];
}

PlayerSkillMgr::~PlayerSkillMgr()
{
}

void PlayerSkillMgr::UpdatePhysics(float fElapsedTime)
{  

}

void PlayerSkillMgr::Update(float fElapsedTime)
{
	for (int x = 0; x < SKILL_SELECTED; ++x)
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

	for (int x = 0; x < SKILL_SELECTED; ++x)
	{
		if (m_skill[x].isActive)
		{
			m_skill[x].skillEffect->Update(fElapsedTime);

		}
	}
}

void PlayerSkillMgr::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	for (int x = 0; x < SKILL_SELECTED; ++x)
	{
		if (!m_skill[x].isActive) continue;

		XMFLOAT4X4 xmf4x4World;
		XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_skill[x].skillEffect->GetTransform().GetWorldMatrix()))); 
		pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &xmf4x4World, 0);
		m_skill[x].skillEffect->Render(pd3dCommandList, isGBuffers);
	}
}

void PlayerSkillMgr::Activate(PlayerStatus* MPstaus, int index)
{ 
	if (m_skill[index].isActive) return;
	if (m_skill[index].RemainCoolTime > 0.f) return;
	if ((MPstaus->m_Guage - 10/*사용하는 마나 게이지*/) < 0.f) return;

	MPstaus->m_Guage -= 10.f;
	m_skill[index].isActive = true;
	m_skill[index].RemainCoolTime = m_skill[index].skillEffect->m_CoolTime;
	m_skill[index].spawnPosition =
		PlayerManager::GetMainPlayer()->GetTransform().GetPosition();

	m_skill[index].skillEffect->SetVelocity(
		m_skill[index].spawnPosition,
		75,
		PlayerManager::GetMainPlayer()->GetTransform().GetLook()
	); 
}

void PlayerSkillMgr::Deactive(int index)
{ 
	m_skill[index].isActive = false;
}

SelectableSkill * PlayerSkillMgr::GetSkillEffect(int index)
{
	return &m_skill[index];
}

void PlayerSkillMgr::SetSkillEffect(SkillEffect * skilleffect, UINT index)
{
	m_skill[index].skillEffect = skilleffect;
}

void PlayerSkillMgr::Deactivate()
{
	for (int x = 0; x < SKILL_SELECTED; ++x)
	{
		if (!m_skill[x].isActive) continue;
		 
		if (Vector3::Length(m_skill[x].spawnPosition, m_skill[x].skillEffect->GetTransform().GetPosition()) > m_distance)
		{
			m_skill[x].isActive = false;
		}
	}
}
 