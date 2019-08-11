#include "stdafx.h"
#include "GameObject.h"
#include "PlayerMovement.h"

#include "SoundManager.h"

#define RUN_MAX_VELOCITY 950.F

void PlayerMovement::MoveVelocity(const XMFLOAT3 & xmf3Shift)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
}

PlayerMovement::PlayerMovement(GameObject * pOwner)
	:Movement(pOwner)
{
	RunMode(); 
}

PlayerMovement::~PlayerMovement()
{

}
 
void PlayerMovement::MoveVelocity(DWORD dwDirection, float fTimeElapsed)
{
	// 사운드 ///////////////////////////////////////////////////////////
	SoundManager::GetInstance()->Play(ENUM_SOUND::PLAYER_MOVE);
	// 사운드 ///////////////////////////////////////////////////////////

	AXIS axis = AXIS{ m_pOwner->GetTransform().GetCoorAxis() };

	XMFLOAT3 xmf3Shift = XMFLOAT3(0.f, 0.f, 0.f); // 이동량

	// 플레이어를 dwDirection 방향으로 이동한다(실제로는 속도 벡터를 변경한다).
	// 이동 거리는 시간에 비례하도록 한다. 플레이어의 이동 속력은 (20m/초)로 가정한다.
	float fDistance = m_fDistance * fTimeElapsed; // 1초당 최대 속력 20m으로 가정, 현재 1 = 1cm

	if (m_isBroomMode) // 만약 빗자루 모드일 경우...
	{ 
		SoundManager::GetInstance()->Stop(ENUM_SOUND::PLAYER_MOVE);

		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, -fDistance);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, -fDistance);

		MoveVelocity(xmf3Shift);
	}
	else
	{ 
		SoundManager::GetInstance()->Stop(ENUM_SOUND::PLAYER_MOVE);

		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, m_fMaxVelocityXZ - 500);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, axis.look, -m_fMaxVelocityXZ + 500);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, m_fMaxVelocityXZ - 500);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, axis.right, -m_fMaxVelocityXZ + 500);

		m_xmf3Velocity = xmf3Shift;
	}

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
	m_fMaxVelocityXZ = RUN_MAX_VELOCITY;
	m_fFriction = 100000.0f;
	m_fDistance = 0.f;
}
