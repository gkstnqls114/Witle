#include "stdafx.h"

#include "Movement.h"

#include "SkillEffect.h"

void SkillEffect::ReleaseMembers()
{
	delete m_Movement;
	m_Movement = nullptr;
}

void SkillEffect::ReleaseMemberUploadBuffers()
{
}

void SkillEffect::UpdateMovement()
{
}
 
SkillEffect::SkillEffect(const std::string & entityID, float distance)
	:GameObject(entityID)
{
	m_Movement = new Movement(this);
	m_Movement->m_fDistance = distance;
}

SkillEffect::~SkillEffect()
{
}

void SkillEffect::Update(float fElapsedTime)
{
	m_Movement->Update(fElapsedTime); // �̵����� ����Ѵ�.

	// �̵�����ŭ �����δ�. 
	Move(Vector3::ScalarProduct(m_Movement->m_xmf3Velocity, fElapsedTime, false));
}
 
void SkillEffect::Move(const XMFLOAT3 & xmf3Shift)
{
	m_Transform.Move(xmf3Shift);
	m_Transform.Update(0.f);
}

void SkillEffect::SetVelocity(const XMFLOAT3 & position, const float OffsetY, const XMFLOAT3 & direction)
{
	m_Transform.SetIdentity();
	m_Transform.SetPosition(Vector3::Add(position, Vector3::ScalarProduct(XMFLOAT3(0, 1, 0), OffsetY, false)));
	m_Transform.Update(0.f);
	m_Movement->m_xmf3Velocity =
		Vector3::ScalarProduct(Vector3::Normalize(direction), m_Movement->m_fDistance, false);
}

 