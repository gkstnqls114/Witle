#include "stdafx.h"

#include "Object.h"
#include "PlayerManager.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "MonsterStatus.h"
#include "Monster.h" 

#include "GeneralMonsterActionMgr.h"

#include "SoundManager.h"

void GeneralMonsterActionMgr::UpdateState(float fElpasedTime)
{
	m_TotalTime += fElpasedTime;

	// Dead 상태만 제외하고 공통적으로 들어가야함
	if (m_AfterAction != &m_DeadAction && static_cast<Monster*>(m_pOwner)->GetStatus()->GetGuage() <= 0)
	{
		ChangeStateToDead();
	}

	static_cast<GeneralMonsterAction*>(m_AfterAction)->UpdateState(fElpasedTime, this);

	// CHASE에서 IDLE로...
	//if (m_AfterAction == &m_ChaseAction
	//	&& !IsNearPlayer(PlayerManager::GetMainPlayer(), pMonsterOwner->GetRecognitionRange()->m_RecognitionRange))
	//{
	//	static_cast<Monster*>(m_pOwner)->SetAnimationID(SPACECAT_IDLE.ID); 
	//	m_TotalTime = 0.f;
	//	m_AfterAction = &m_IdleAction;
	//	return;
	//} 
}

bool GeneralMonsterActionMgr::isDifferAction() const
{
	return ((static_cast<GeneralMonsterAction*>(m_BeforeAction)->m_ActionID) != (static_cast<GeneralMonsterAction*>(m_AfterAction)->m_ActionID));
}

bool GeneralMonsterActionMgr::isDifferAfterAndCurrent() const
{
	return ((static_cast<GeneralMonsterAction*>(m_AfterAction)->m_ActionID) != (static_cast<GeneralMonsterAction*>(m_CurrAction)->m_ActionID));
}

void GeneralMonsterActionMgr::Init()
{
	m_BeforeAction = &m_GeneralMonsterErrorAction;
	m_CurrAction = &m_GeneralMonsterErrorAction;
	m_AfterAction = &m_IdleAction;
}

void GeneralMonsterActionMgr::ChangeStateBefore()
{ 
	// 이전 액션과 현재 액션이 같다면..
	if (static_cast<GeneralMonsterAction*>(m_AfterAction)->m_ActionID
		== static_cast<GeneralMonsterAction*>(m_BeforeAction)->m_ActionID)
	{
		assert(false);
	}

	// 상태가 이전하고 다른 경우에만 서로 변경한다.
	if (m_AfterAction != m_BeforeAction)
	{
		Action* temp = m_AfterAction;
		m_AfterAction = m_BeforeAction;
		m_BeforeAction = temp;
	}

	if (m_AfterAction == &m_IdleAction)
	{
		ChangeStateToIdle();
	}
	else if (m_AfterAction == &m_MoveAction)
	{
		ChangeStateToMove();
	}
	else if (m_AfterAction == &m_ChaseAction)
	{
		ChangeStateToChase();
	}
	else if (m_AfterAction == &m_SearchAction)
	{
		ChangeStateToSearch();
	}
	else if (m_AfterAction == &m_DeadAction)
	{
		ChangeStateToDead();
	}
	else if (m_AfterAction == &m_HitAction)
	{
		ChangeStateToHit();
	}
	else if (m_AfterAction == &m_AttackAction)
	{
		ChangeStateToAttack();
	}
}

void GeneralMonsterActionMgr::ChangeStateToIdle()
{
	bool isChanged = ChangeAction(&m_IdleAction);
	if (!isChanged) return;
	m_AfterAction->Init();

	static_cast<Monster*>(m_pOwner)->SetAnimationID(m_IdleAction.m_AnimationID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void GeneralMonsterActionMgr::ChangeStateToMove()
{ 
	bool isChanged = ChangeAction(&m_MoveAction);
	if (!isChanged) return;
	m_AfterAction->Init();

	static_cast<Monster*>(m_pOwner)->SetAnimationID(m_MoveAction.m_AnimationID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void GeneralMonsterActionMgr::ChangeStateToChase()
{ 
	bool isChanged = ChangeAction(&m_ChaseAction);
	if (!isChanged) return;
	m_AfterAction->Init();

	static_cast<Monster*>(m_pOwner)->SetAnimationID(m_ChaseAction.m_AnimationID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void GeneralMonsterActionMgr::ChangeStateToSearch()
{ 
	bool isChanged = ChangeAction(&m_SearchAction);
	if (!isChanged) return;
	m_AfterAction->Init();

	static_cast<Monster*>(m_pOwner)->SetAnimationID(m_SearchAction.m_AnimationID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void GeneralMonsterActionMgr::ChangeStateToDead()
{ 
	bool isChanged = ChangeAction(&m_DeadAction);
	if (!isChanged) return;
	m_AfterAction->Init();

	static_cast<Monster*>(m_pOwner)->SetAnimationID(m_DeadAction.m_AnimationID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false); 
}

void GeneralMonsterActionMgr::ChangeStateToHit()
{ 
	bool isChanged = ChangeAction(&m_HitAction);
	if (!isChanged) return;
	m_AfterAction->Init();

	static_cast<Monster*>(m_pOwner)->SetAnimationID(m_HitAction.m_AnimationID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void GeneralMonsterActionMgr::ChangeStateToAttack()
{ 
	bool isChanged = ChangeAction(&m_AttackAction);
	if (!isChanged) return;
	m_AfterAction->Init();

	static_cast<Monster*>(m_pOwner)->SetAnimationID(m_AttackAction.m_AnimationID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true); 
}
