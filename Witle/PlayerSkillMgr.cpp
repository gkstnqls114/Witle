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
	m_skill = new SelectableSkill*[SKILL_SELECTED];
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
		m_skill[x]->Update(fElapsedTime);
	}
}

void PlayerSkillMgr::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	for (int x = 0; x < SKILL_SELECTED; ++x)
	{
		if (!m_skill[x]->isActive) continue;

		XMFLOAT4X4 xmf4x4World;
		XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_skill[x]->m_skillEffect->GetTransform().GetWorldMatrix()))); 
		pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &xmf4x4World, 0);
		m_skill[x]->m_skillEffect->Render(pd3dCommandList, isGBuffers);
	}
}

void PlayerSkillMgr::Activate(PlayerStatus* MPstaus, int index)
{ 
	if (m_skill[index]->isActive) return;
	if (m_skill[index]->RemainCoolTime > 0.f) return;
	if ((MPstaus->GetGuage() - m_skill[index]->m_UsingMp/*사용하는 마나 게이지*/) < 0.f) return;

	m_skill[index]->Active();

	MPstaus->SubstractHP(m_skill[index]->m_UsingMp + PlayerManager::GetMainPlayer()->shield);
	m_skill[index]->isActive = true;
	m_skill[index]->RemainCoolTime = m_skill[index]->GetCoolTime();
	m_skill[index]->spawnPosition =
		PlayerManager::GetMainPlayer()->GetTransform().GetPosition();

	m_skill[index]->m_skillEffect->SetVelocity(
		m_skill[index]->spawnPosition,
		75,
		PlayerManager::GetMainPlayer()->GetTransform().GetLook()
	); 
}

void PlayerSkillMgr::Deactive(int index)
{ 
	m_skill[index]->isActive = false;
}

SelectableSkill * PlayerSkillMgr::GetpSelectableSkill(int index)
{
	return m_skill[index];
}

void PlayerSkillMgr::SetSkill(SelectableSkill * skilleffect, UINT index)
{
	m_skill[index] = skilleffect;
}

bool PlayerSkillMgr::isActive(ENUM_SELECTABLESKILL type)
{
	for (int x = 0; x < SKILL_SELECTED; ++x)
	{
		if (m_skill[x]->GetSelectableSkillType() == type)
		{
			return m_skill[x]->isActive;
		}
	}
	return false;
}
