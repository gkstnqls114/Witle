#include "stdafx.h"
#include "SkillEffect.h"
#include "BallSkill.h"


bool BallSkill::IsFinish()
{
	return (Vector3::Length(spawnPosition, m_skillEffect->GetTransform().GetPosition()) > m_distance);
}
 
BallSkill::BallSkill(const std::string& entityID, ENUM_SELECTABLESKILL skilltype, float cooltime)
	:SelectableSkill(entityID, skilltype, cooltime, 10.f)
{
}


BallSkill::~BallSkill()
{
}
