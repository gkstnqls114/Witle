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

#include "SkillStg.h"

SkillStg* SkillStg::m_Instance{ nullptr };

SkillStg::SkillStg()
{
	for (int index = 0; index < SKILL_TO_CHOOSE; ++index)
	{
		m_SelectableSkills[index] = nullptr;
	}
}

SkillStg::~SkillStg()
{
}

void SkillStg::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_SelectableSkills[ENUM_SELECTABLESKILL::SELECTABLESKILL_FIREBALL] = new FireBallSkill(pd3dDevice, pd3dCommandList);
	m_SelectableSkills[ENUM_SELECTABLESKILL::SELECTABLESKILL_ICEBALL] = new IceBallSkill(pd3dDevice, pd3dCommandList);
	m_SelectableSkills[ENUM_SELECTABLESKILL::SELECTABLESKILL_LIGHTNINGBALL] = new LightningBallSkill(pd3dDevice, pd3dCommandList);

	m_SelectableSkills[ENUM_SELECTABLESKILL::SELECTABLESKILL_SHIELD] = new Shield(pd3dDevice, pd3dCommandList);
	m_SelectableSkills[ENUM_SELECTABLESKILL::SELECTABLESKILL_BLESSING] = new Blessing(pd3dDevice, pd3dCommandList);
	m_SelectableSkills[ENUM_SELECTABLESKILL::SELECTABLESKILL_HEALING] = new Healing(pd3dDevice, pd3dCommandList);
 }

void SkillStg::ReleaseUploadBuffers()
{
	for (int index = 0; index < SKILL_TO_CHOOSE; ++index)
	{ 
		if (m_SelectableSkills[index])
		{
			m_SelectableSkills[index]->ReleaseUploadBuffers();
		}
	}
}

void SkillStg::ReleaseObjects()
{
	for (int index = 0; index < SKILL_TO_CHOOSE; ++index)
	{
		if (m_SelectableSkills[index])
		{
			m_SelectableSkills[index]->ReleaseObjects();
			delete m_SelectableSkills[index];
			m_SelectableSkills[index] = nullptr;
		}
	}
}

SelectableSkill * SkillStg::GetSkill(int index) const
{
	return m_SelectableSkills[index];
}
