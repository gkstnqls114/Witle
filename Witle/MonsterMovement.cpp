#include "stdafx.h"

#include "GeneralMonsterActionMgr.h"
#include "BossMonsterActionMgr.h"

#include "Monster.h"
#include "RecognitionRange.h"
#include "Player.h"
#include "MonsterMovement.h"

#include "SoundManager.h"

 
void MonsterMovement::ReleaseObjects()
{ 
}

void MonsterMovement::MoveVelocity(const XMFLOAT3 & xmf3Shift)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
}

MonsterMovement::MonsterMovement(GameObject * pOwner)
	:Movement(pOwner)
{

}

MonsterMovement::~MonsterMovement()
{

}

void MonsterMovement::Update(float fTimeElapsed)
{
	Movement::Update(fTimeElapsed);
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
