#include "stdafx.h"
#include "SkillEffect.h"
#include "BallSkill.h"


bool BallSkill::IsFinish()
{
	return (Vector3::Length(spawnPosition, m_skillEffect->GetTransform().GetPosition()) > m_distance);
}

void BallSkill::UpdateActive(float fElapsedTime)
{
}

BallSkill::BallSkill()
{
}


BallSkill::~BallSkill()
{
}
