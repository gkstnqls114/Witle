#include "stdafx.h"

// Action ////////////////////////
#include "IdleAction.h"
#include "MoveAction.h"
#include "ChaseAction.h"
#include "SearchAction.h"
#include "PlayerManager.h"
#include "DeadAction.h"
// Action ////////////////////////

#include "Player.h"
#include "MonsterMovement.h"

bool MonsterMovement::IsNearPlayer(const Player * target, float distance)
{
	return Vector3::Length(Vector3::Subtract(m_pOwner->GetTransform().GetPosition(), target->GetTransform().GetPosition())) < distance;
}

void MonsterMovement::UpdateVelocity(float fTimeElapsed)
{ 
	m_CurrMonsterAction->UpdateVelocity(fTimeElapsed, this); 
}

void MonsterMovement::ReleaseObjects()
{ 
	if (m_IdleAction)        delete m_IdleAction;
	if (m_MoveAction)        delete m_MoveAction;
	if (m_ChaseAction)       delete m_ChaseAction;
	if (m_SearchAction)      delete m_SearchAction;
	if (m_DeadAction)        delete m_DeadAction;
}

void MonsterMovement::MoveVelocity(const XMFLOAT3 & xmf3Shift)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
}

MonsterMovement::MonsterMovement(GameObject * pOwner)
	:ComponentBase(pOwner)
{
	m_IdleAction = new IdleAction(pOwner);
	m_MoveAction = new MoveAction(pOwner);
	m_ChaseAction = new ChaseAction(pOwner);
	m_SearchAction = new SearchAction(pOwner);
	m_DeadAction = new DeadAction(pOwner);

	m_CurrMonsterAction = m_IdleAction;
}

MonsterMovement::~MonsterMovement()
{

}

void MonsterMovement::Update(float fTimeElapsed)
{
	UpdateVelocity(fTimeElapsed);

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

void MonsterMovement::UpdateState(float fElpasedTime, const XMFLOAT3& randomDirection, float distance)
{   
	// 어떤 상황이던간에 근처에 플레이어가 있는 경우 따라간다.
	if (m_CurrMonsterAction != m_ChaseAction &&
		IsNearPlayer(PlayerManager::GetMainPlayer(), distance))
	{
		m_TotalTime = 0.f;
		m_CurrMonsterAction = m_ChaseAction;
		return;
	}
	
	if (m_CurrMonsterAction == m_ChaseAction
		&& !IsNearPlayer(PlayerManager::GetMainPlayer(), distance))
	{
		m_TotalTime = 0.f;
		m_CurrMonsterAction = m_IdleAction;
		return;
	}

	if (m_CurrMonsterAction == m_ChaseAction) return;

	m_TotalTime += fElpasedTime;
	if (m_CurrMonsterAction == m_IdleAction)
	{
		if (m_TotalTime > m_IdleTime)
		{
			m_TotalTime = 0.f;
			m_CurrMonsterAction = m_MoveAction;
			return;
		}
	}

	if (m_CurrMonsterAction == m_MoveAction)
	{ 
		if (m_TotalTime > m_MoveTime)
		{
			m_TotalTime = 0.f;
			m_CurrMonsterAction = m_IdleAction;
			return;
		}
	}

}

XMFLOAT3 MonsterMovement::AlreadyUpdate(float fTimeElapsed)
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
 

void MonsterMovement::ReduceVelocity(float fTimeElapsed)
{
	if (Vector3::Length(m_xmf3Velocity) <= 0.f)
	{
		m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
		return; // 움직이지 않는 상태일 경우 그냥 넘어간다.
	}
	  
	MoveVelocity(Vector3::ScalarProduct(m_xmf3Velocity, -0.2f, false));
	
}
 