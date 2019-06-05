#include "stdafx.h"

#include "Object.h"
#include "PlayerManager.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "MonsterStatus.h"
#include "Monster.h"

#include "MonsterActionMgr.h"
  

void MonsterActionMgr::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	m_CurrMonsterAction->UpdateVelocity(fElpasedTime, movement);
}

void MonsterActionMgr::UpdateState(float fElpasedTime)
{ 
	m_TotalTime += fElpasedTime;
	 
	// Dead 상태만 제외하고 공통적으로 들어가야함
	if (m_CurrMonsterAction != &m_DeadAction && static_cast<Monster*>(m_pOwner)->GetStatus()->m_HP <= 0)
	{
		ChangeStateToDead();
	}

	m_CurrMonsterAction->UpdateState(fElpasedTime, this);
	    
	// CHASE에서 IDLE로...
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
	MonsterAction* temp = m_CurrMonsterAction;
	m_CurrMonsterAction = m_BeforeMonsterAction;
	m_BeforeMonsterAction = temp;
	 
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
	m_BeforeMonsterAction = m_CurrMonsterAction;
	m_CurrMonsterAction = &m_IdleAction;
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_IDLE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void MonsterActionMgr::ChangeStateToMove()
{
	m_BeforeMonsterAction = m_CurrMonsterAction;
	m_CurrMonsterAction = &m_MoveAction;
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void MonsterActionMgr::ChangeStateToChase()
{
	m_BeforeMonsterAction = m_CurrMonsterAction;
	m_CurrMonsterAction = &m_ChaseAction;
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void MonsterActionMgr::ChangeStateToSearch()
{
	m_BeforeMonsterAction = m_CurrMonsterAction;
	m_CurrMonsterAction = &m_SearchAction;
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void MonsterActionMgr::ChangeStateToDead()
{
	m_BeforeMonsterAction = m_CurrMonsterAction;
	m_CurrMonsterAction = &m_DeadAction;
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_DEAD.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void MonsterActionMgr::ChangeStateToHit()
{
	m_BeforeMonsterAction = m_CurrMonsterAction;
	m_CurrMonsterAction = &m_HitAction;
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_HIT.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void MonsterActionMgr::ChangeStateToAttack()
{
	m_BeforeMonsterAction = m_CurrMonsterAction;
	m_CurrMonsterAction = &m_AttackAction;
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_ATTACK.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true);
}
