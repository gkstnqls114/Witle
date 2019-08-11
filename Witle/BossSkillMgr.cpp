#include "stdafx.h"

#include "SkillEffect.h"
// Selectable Skill 관련 //////////////////////////// 
#include "Skill.h"
#include "FireBallSkill.h"
#include "IceBallSkill.h"
#include "LightningBallSkill.h"
#include "Shield.h"
#include "Blessing.h"
#include "Healing.h"
// Selectable Skill 관련 ////////////////////////////

#include "Dragon.h"
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
	for (int x = 0; x < m_count; ++x)
	{
		m_SelectableSkills[x]->Update(fElapsedTime);
	}
}

void BossSkillMgr::Render(ID3D12GraphicsCommandList * pd3dCommandList, bool isGBuffers)
{
	for (int x = 0; x < m_count; ++x)
	{
		if (!m_SelectableSkills[x]->isActive) continue;

		XMFLOAT4X4 xmf4x4World;
		XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_SelectableSkills[x]->m_skillEffect->GetTransform().GetWorldMatrix())));
		pd3dCommandList->SetGraphicsRoot32BitConstants(ROOTPARAMETER_WORLD, 16, &xmf4x4World, 0);
		m_SelectableSkills[x]->m_skillEffect->Render(pd3dCommandList, isGBuffers);
	}
}

void BossSkillMgr::Activate(Dragon* boss, int index)
{
	if (m_SelectableSkills[index]->isActive) return;

	m_SelectableSkills[index]->isActive = true;
	m_SelectableSkills[index]->Active();
	  
	m_SelectableSkills[index]->RemainCoolTime = m_SelectableSkills[index]->GetCoolTime();
	m_SelectableSkills[index]->spawnPosition =
		boss->GetTransform().GetPosition();

	m_SelectableSkills[index]->m_skillEffect->SetVelocity(
		m_SelectableSkills[index]->spawnPosition,
		75,
		boss->GetTransform().GetLook()
	);
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
	for (int x = 0; x < m_count; ++x)
	{
		if (m_SelectableSkills[x]->GetSelectableSkillType() == type)
		{
			return m_SelectableSkills[x]->isActive;
		}
	}
	return false;
}
