#include "stdafx.h"
#include "Skill.h"

Skill::~Skill()
{
}

void Skill::Update(float fTimeElapsed)
{
	if (!m_isUsing) return;
	if (m_UsingTime < m_SkillTime)
	{
		DoNotUse(); 
	}

	m_SkillTime += fTimeElapsed; 
}
