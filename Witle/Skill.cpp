#include "stdafx.h"
#include "SkillEffect.h"
#include "Skill.h"

FixedSkill::~FixedSkill()
{
}

void FixedSkill::Update(float fTimeElapsed)
{
	if (!m_isUsing) return;
	if (m_UsingTime < m_SkillTime)
	{
		DoNotUse(); 
	}

	m_SkillTime += fTimeElapsed; 
}

void SelectableSkill::ReleaseMembers()
{
	if (m_skillEffect)
	{
		m_skillEffect->ReleaseObjects();
		delete m_skillEffect;
		m_skillEffect = nullptr;
	}
}

void SelectableSkill::ReleaseMemberUploadBuffers()
{
	if (m_skillEffect) m_skillEffect->ReleaseUploadBuffers(); 
}

SelectableSkill::~SelectableSkill()
{
}
