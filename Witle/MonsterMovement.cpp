#include "stdafx.h"

#include "GeneralMonsterActionMgr.h"
#include "BossMonsterActionMgr.h"

#include "Monster.h"
#include "RecognitionRange.h"
#include "Player.h"
#include "MonsterMovement.h"

#include "SoundManager.h"


void MonsterMovement::UpdateVelocity(float fTimeElapsed)
{ 
	m_MonsterActionMgr->UpdateVelocity(fTimeElapsed, this);
}

void MonsterMovement::ReleaseObjects()
{ 
	if (m_MonsterActionMgr)        delete m_MonsterActionMgr;
}

void MonsterMovement::MoveVelocity(const XMFLOAT3 & xmf3Shift)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
}

MonsterMovement::MonsterMovement(GameObject * pOwner, float idleTime, float moveTime, bool isBoss)
	:Movement(pOwner)
{
	if (!isBoss)
	{
		m_MonsterActionMgr = new GeneralMonsterActionMgr(pOwner, idleTime, moveTime);

	}
	else
	{ 
		m_MonsterActionMgr = new BossMonsterActionMgr(pOwner);
	}
}

MonsterMovement::~MonsterMovement()
{

}

void MonsterMovement::Update(float fTimeElapsed)
{
	UpdateVelocity(fTimeElapsed); // State 상태에 따라 Velocity를 갱신(Set)한다.

	Movement::Update( fTimeElapsed); 
}

void MonsterMovement::UpdateState(float fElpasedTime)
{   
	m_MonsterActionMgr->UpdateState(fElpasedTime);
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
 