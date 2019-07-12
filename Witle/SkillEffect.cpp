#include "stdafx.h"

#include "Movement.h"

#include "SkillEffect.h"

void SkillEffect::UpdateMovement()
{
}

SkillEffect::SkillEffect(const std::string & entityID)
	:GameObject(entityID)
{
	m_Movement = new Movement(this);
}

SkillEffect::~SkillEffect()
{
}

void SkillEffect::Update(float)
{

}

void SkillEffect::Move(ULONG nDirection, float fDistance, bool bVelocity)
{
}

void SkillEffect::Move(const XMFLOAT3 & xmf3Shift)
{
	m_Transform.Move(xmf3Shift);
}

void SkillEffect::MoveVelocity(const XMFLOAT3 & xmf3Shift)
{
}

void SkillEffect::Move(float fxOffset, float fyOffset, float fzOffset)
{
}

void SkillEffect::Rotate(float x, float y, float z)
{

}
