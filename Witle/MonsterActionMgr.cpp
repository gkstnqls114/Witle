#include "stdafx.h"

#include "Object.h"
#include "PlayerManager.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "MonsterStatus.h"
#include "Monster.h"

#include "MonsterActionMgr.h"
  

void MonsterActionMgr::ChangeAction(MonsterAction * action)
{
	// ���� ���¿� �����ϴٸ� �ٲ��� �ʴ´�.
	if (m_CurrMonsterAction == action) return;

	m_BeforeMonsterAction = m_CurrMonsterAction;
	m_CurrMonsterAction = action;
}

void MonsterActionMgr::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	m_CurrMonsterAction->UpdateVelocity(fElpasedTime, movement);
}

void MonsterActionMgr::UpdateState(float fElpasedTime)
{ 
	m_TotalTime += fElpasedTime;
	 
	// Dead ���¸� �����ϰ� ���������� ������
	if (m_CurrMonsterAction != &m_DeadAction && static_cast<Monster*>(m_pOwner)->GetStatus()->m_HP <= 0)
	{
		ChangeStateToDead();
	}

	m_CurrMonsterAction->UpdateState(fElpasedTime, this);
	    
	// CHASE���� IDLE��...
	//if (m_CurrMonsterAction == &m_ChaseAction
	//	&& !IsNearPlayer(PlayerManager::GetMainPlayer(), pMonsterOwner->GetRecognitionRange()->m_RecognitionRange))
	//{
	//	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_IDLE.ID); 
	//	m_TotalTime = 0.f;
	//	m_CurrMonsterAction = &m_IdleAction;
	//	return;
	//} 
}

void MonsterActionMgr::ChangeStateBefore()
{ 
	// ���°� �����ϰ� �ٸ� ��쿡�� ���� �����Ѵ�.
	if (m_CurrMonsterAction != m_BeforeMonsterAction)
	{
		MonsterAction* temp = m_CurrMonsterAction;
		m_CurrMonsterAction = m_BeforeMonsterAction;
		m_BeforeMonsterAction = temp;
	}

	if (m_CurrMonsterAction == &m_IdleAction)
	{
		ChangeStateToIdle();
	}
	else if (m_CurrMonsterAction == &m_MoveAction)
	{
		ChangeStateToMove();
	}
	else if (m_CurrMonsterAction == &m_ChaseAction)
	{
		ChangeStateToChase();
	}
	else if (m_CurrMonsterAction == &m_SearchAction)
	{ 
		ChangeStateToSearch();
	}
	else if (m_CurrMonsterAction == &m_DeadAction)
	{
		ChangeStateToDead();
	} 
	else if (m_CurrMonsterAction == &m_HitAction)
	{
		ChangeStateToHit();
	}
	else if (m_CurrMonsterAction == &m_AttackAction)
	{
		ChangeStateToAttack();
	}
}

void MonsterActionMgr::ChangeStateToIdle()
{
	ChangeAction(&m_IdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_IDLE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void MonsterActionMgr::ChangeStateToMove()
{
	ChangeAction(&m_MoveAction); 
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void MonsterActionMgr::ChangeStateToChase()
{
	ChangeAction(&m_ChaseAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void MonsterActionMgr::ChangeStateToSearch()
{
	ChangeAction(&m_SearchAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void MonsterActionMgr::ChangeStateToDead()
{
	ChangeAction(&m_DeadAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_DEAD.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void MonsterActionMgr::ChangeStateToHit()
{
	ChangeAction(&m_HitAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_HIT.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void MonsterActionMgr::ChangeStateToAttack()
{
	ChangeAction(&m_AttackAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_ATTACK.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true);
}
