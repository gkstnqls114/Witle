#include "stdafx.h"
#include "GameObject.h"
#include "Movement.h"

#define RUN_MAX_VELOCITY 1500.F

void Movement::MoveVelocity(const XMFLOAT3 & xmf3Shift)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
}

Movement::Movement(GameObject * pOwner)
	:ComponentBase(pOwner)
{ 
}

Movement::~Movement()
{

}

void Movement::Update(float fTimeElapsed)
{ 
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, m_xmf3Gravity);
	float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.z * m_xmf3Velocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ;
	if (fLength > m_fMaxVelocityXZ)
	{
		m_xmf3Velocity.x *= (fMaxVelocityXZ / fLength);
		m_xmf3Velocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_fMaxVelocityY;
	fLength = sqrtf(m_xmf3Velocity.y * m_xmf3Velocity.y);
	if (fLength > m_fMaxVelocityY) m_xmf3Velocity.y *= (fMaxVelocityY / fLength);
}

XMFLOAT3 Movement::AlreadyUpdate(float fTimeElapsed)
{   
	if (Vector3::Length(m_xmf3Velocity) <= 0.f) return XMFLOAT3(0.0f, 0.0f, 0.0f); // 움직이지 않는 상태일 경우 그냥 넘어간다.

	XMFLOAT3 AlreadyVelocity = Vector3::Add(m_xmf3Velocity, m_xmf3Gravity);
	float fLength = sqrtf(AlreadyVelocity.x * AlreadyVelocity.x + AlreadyVelocity.z * AlreadyVelocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ;
	if (fLength > m_fMaxVelocityXZ)
	{
		AlreadyVelocity.x *= (fMaxVelocityXZ / fLength);
		AlreadyVelocity.z *= (fMaxVelocityXZ / fLength);
	}
	float fMaxVelocityY = m_fMaxVelocityY;
	fLength = sqrtf(AlreadyVelocity.y * AlreadyVelocity.y);
	if (fLength > m_fMaxVelocityY) AlreadyVelocity.y *= (fMaxVelocityY / fLength);

	// 한 프레임에 해당하여 이동..
	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(AlreadyVelocity, fTimeElapsed, false);
	 
	return xmf3Velocity;
}
 