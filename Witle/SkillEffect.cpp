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
 
SkillEffect::SkillEffect(const std::string & entityID, float cooltime, float distance)
	:GameObject(entityID) , m_CoolTime(cooltime)
{
	m_Movement = new Movement(this);
	m_Movement->m_fDistance = distance;
	m_Movement->m_fMaxVelocityXZ = distance;
}

SkillEffect::~SkillEffect()
{
}

// 위치를 업데이트합니다.
void SkillEffect::Update(float fElapsedTime)
{
	// 이동량을 계산한다.
	m_Movement->Update(fElapsedTime); 

	// 이동량만큼 움직인다. 
	Move(Vector3::ScalarProduct(m_Movement->m_xmf3Velocity, fElapsedTime, false));

	// 업데이트된 포지션에 맞추어 Collider 위치를 수정한다.
	UpdateCollider();
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

 