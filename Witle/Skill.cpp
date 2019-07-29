#include "stdafx.h"
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

SelectableSkill::~SelectableSkill()
{
}
