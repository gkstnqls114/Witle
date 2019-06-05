#include "stdafx.h"

#include "GeneralMonsterActionMgr.h"

#include "Monster.h"
#include "RecognitionRange.h"
#include "Player.h"
#include "MonsterMovement.h"


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

MonsterMovement::MonsterMovement(GameObject * pOwner, float idleTime, float moveTime)
	:Movement(pOwner)
{
	m_MonsterActionMgr = new GeneralMonsterActionMgr(pOwner, idleTime, moveTime);
}

MonsterMovement::~MonsterMovement()
{

}

void MonsterMovement::Update(float fTimeElapsed)
{
	UpdateVelocity(fTimeElapsed); // State ���¿� ���� Velocity�� ����(Set)�Ѵ�.

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
		return; // �������� �ʴ� ������ ��� �׳� �Ѿ��.
	}
	  
	MoveVelocity(Vector3::ScalarProduct(m_xmf3Velocity, -0.2f, false));
	
}
 