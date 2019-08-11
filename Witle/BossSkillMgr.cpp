#include "stdafx.h"

#include "SkillEffect.h"
// Selectable Skill ���� //////////////////////////// 
#include "Skill.h"
#include "FireBallSkill.h"
#include "IceBallSkill.h"
#include "LightningBallSkill.h"
#include "Shield.h"
#include "Blessing.h"
#include "Healing.h"
// Selectable Skill ���� ////////////////////////////

#include "BossSkillMgr.h"

BossSkillMgr* BossSkillMgr::m_Instance{ nullptr };

BossSkillMgr::BossSkillMgr()
{
}

BossSkillMgr::~BossSkillMgr()
{
}

void BossSkillMgr::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_SelectableSkills = new SelectableSkill*[m_count];
	for (int i = 0; i < m_count; ++i)
	{
		m_SelectableSkills[i] = new FireBallSkill(pd3dDevice, pd3dCommandList);
	}

}

void BossSkillMgr::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_count; ++i)
	{
		m_SelectableSkills[i]->ReleaseUploadBuffers();
	} 
}

void BossSkillMgr::ReleaseObjects()
{
	for (int i = 0; i < m_count; ++i)
	{
		m_SelectableSkills[i]->ReleaseObjects();
		delete m_SelectableSkills[i];
		m_SelectableSkills[i] = nullptr;
	} 
	delete[] m_SelectableSkills;
	m_SelectableSkills = nullptr;
}
void BossSkillMgr::UpdatePhysics(float fElapsedTime)
{

}

void BossSkillMgr::Update(float fElapsedTime)
{
	for (int x = 0; x < SKILL_SELECTED; ++x)
	{
		m_SelectableSkills[x]->Update(fElapsedTime);
	}
}

void BossSkillMgr::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	for (int x = 0; x < SKILL_SELECTED; ++x)
	{
		if (!m_SelectableSkills[x]->isActive) continue;

		XMFLOAT4X4 xmf4x4World;
		XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_SelectableSkills[x]->m_skillEffect->GetTransform().GetWorldMatrix())));
		pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &xmf4x4World, 0);
		m_SelectableSkills[x]->m_skillEffect->Render(pd3dCommandList, isGBuffers);
	}
}

void BossSkillMgr::Activate(PlayerStatus* MPstaus, int index)
{
	//if (m_SelectableSkills[index]->isActive) return;
	//if (m_skill[index]->RemainCoolTime > 0.f) return;
	//if ((MPstaus->m_Guage - m_skill[index]->m_UsingMp/*����ϴ� ���� ������*/) < 0.f) return;

	//m_skill[index]->Active();

	//MPstaus->m_Guage -= m_skill[index]->m_UsingMp;
	//m_skill[index]->isActive = true;
	//m_skill[index]->RemainCoolTime = m_skill[index]->GetCoolTime();
	//m_skill[index]->spawnPosition =
	//	PlayerManager::GetMainPlayer()->GetTransform().GetPosition();

	//m_skill[index]->m_skillEffect->SetVelocity(
	//	m_skill[index]->spawnPosition,
	//	75,
	//	PlayerManager::GetMainPlayer()->GetTransform().GetLook()
	//);
}

void BossSkillMgr::Deactive(int index)
{
	m_SelectableSkills[index]->isActive = false;
}

SelectableSkill * BossSkillMgr::GetpSelectableSkill(int index)
{
	return m_SelectableSkills[index];
}

void BossSkillMgr::SetSkill(SelectableSkill * skilleffect, UINT index)
{
	m_SelectableSkills[index] = skilleffect;
}

bool BossSkillMgr::isActive(ENUM_SELECTABLESKILL type)
{
	for (int x = 0; x < SKILL_SELECTED; ++x)
	{
		if (m_SelectableSkills[x]->GetSelectableSkillType() == type)
		{
			return m_SelectableSkills[x]->isActive;
		}
	}
	return false;
}
