#include "stdafx.h"
#include "Skill.h"

Skill::~Skill()
{
}

void Skill::Update(float fTimeElapsed)
{
	if (!m_isUsing) return;
	if (m_CooldownTime < m_SkillTime)
	{
		DoNotUse(); 
	}

	m_SkillTime += fTimeElapsed;
	std::cout << m_SkillTime << std::endl;
}
