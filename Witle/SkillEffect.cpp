#include "stdafx.h"

#include "MyCollider.h"
#include "Movement.h"

#include "SkillEffect.h"

void SkillEffect::ReleaseMembers()
{
	if (m_Movement)
	{
		m_Movement->ReleaseObjects();
		delete m_Movement;
	    m_Movement = nullptr;
	}

	if (m_MyCollider)
	{
		m_MyCollider->ReleaseObjects();
		delete m_MyCollider;
		m_MyCollider = nullptr;
	}
}

void SkillEffect::ReleaseMemberUploadBuffers()
{
	if (m_MyCollider)m_MyCollider->ReleaseUploadBuffers();
}

void SkillEffect::UpdateMovement()
{
}
 
SkillEffect::SkillEffect(const std::string & entityID, float cooltime, ENUM_SKILLTYPE skilltype, float distance)
	:GameObject(entityID) , m_Skilltype(skilltype)
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

	XMFLOAT3 shift = Vector3::ScalarProduct(m_Movement->GetVelocity(), fElapsedTime, false);

	// 이동량만큼 움직인다. 
	Move(shift);

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
	m_Movement->SetVelocity(Vector3::ScalarProduct(Vector3::Normalize(direction), m_Movement->m_fDistance, false));
	 
}

 