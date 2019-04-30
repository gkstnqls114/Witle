#include "stdafx.h"
#include "PlayerMovement.h"

PlayerMovement::PlayerMovement(GameObject * pOwner)
	:ComponentBase(pOwner)
{

}

PlayerMovement::~PlayerMovement()
{

}

void PlayerMovement::Update(float fTimeElapsed)
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

XMFLOAT3 PlayerMovement::AlreadyUpdate(float fTimeElapsed)
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

void PlayerMovement::MoveVelocity(const XMFLOAT3 & xmf3Shift)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
}

void PlayerMovement::ReduceVelocity(float fTimeElapsed)
{
	if (Vector3::Length(m_xmf3Velocity) <= 0.f)
	{
		m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
		return; // 움직이지 않는 상태일 경우 그냥 넘어간다.
	}
	
	if (m_isBroomMode)
	{ 
		float fLength = Vector3::Length(m_xmf3Velocity);
		float fDeceleration = (m_fFriction * fTimeElapsed); //해당상수는 Friction
		if (fDeceleration > fLength) fDeceleration = fLength;
		MoveVelocity(Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));
	}
	else
	{ 
		MoveVelocity(Vector3::ScalarProduct(m_xmf3Velocity, -0.2f, false));
	}
}

void PlayerMovement::BroomMode()
{  
	m_isBroomMode = true;
	m_fMaxVelocityXZ = 5000.0f;
	m_fFriction = 3000.0f;
	m_fDistance = 5000.f;
}

void PlayerMovement::RunMode()
{
	m_isBroomMode = false;
	m_fMaxVelocityXZ = 2000.0f; // 1초당 20m
	m_fFriction = 100000.0f;
	m_fDistance = 10000.f;
}
