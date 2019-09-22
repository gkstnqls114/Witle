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

	// Dead ���¸� �����ϰ� ���������� ������
	if (m_AfterAction != &m_DeadAction && static_cast<Monster*>(m_pOwner)->GetStatus()->GetGuage() <= 0)
	{
		ChangeStateToDead();
	}

	static_cast<GeneralMonsterAction*>(m_AfterAction)->UpdateState(fElpasedTime, this);

	// CHASE���� IDLE��...
	//if (m_AfterAction == &m_ChaseAction
	//	&& !IsNearPlayer(PlayerManager::GetMainPlayer(), pMonsterOwner->GetRecognitionRange()->m_RecognitionRange))
	//{
	//	static_cast<Monster*>(m_pOwner)->SetAnimationID(SPACECAT_IDLE.ID); 
	//	m_TotalTime = 0.f;
	//	m_AfterAction = &m_IdleAction;
	//	return;
	//} 
}
 
bool GeneralMonsterActionMgr::isDifferAfterAndCurrent() const
{
	if ((static_cast<GeneralMonsterAction*>(m_AfterAction)->m_ActionID == ENUM_GENERALMONSTER_ACTIONID::GENERALMONSTER_ACTIONID_ERROR) && (static_cast<GeneralMonsterAction*>(m_CurrAction)->m_ActionID == ENUM_GENERALMONSTER_ACTIONID::GENERALMONSTER_ACTIONID_ERROR)) return true;
	if ((static_cast<GeneralMonsterAction*>(m_AfterAction)->m_ActionID) != (static_cast<GeneralMonsterAction*>(m_CurrAction)->m_ActionID)) return true;
	else return false; 
}

void GeneralMonsterActionMgr::SetUpAfterAction()
{
	m_AfterAction = &m_GeneralMonsterErrorAction;
}

bool GeneralMonsterActionMgr::isAfterNoneAction()
{
	return isSameAfterAction(&m_GeneralMonsterErrorAction);
}

void GeneralMonsterActionMgr::Init()
{
	m_BeforeAction = &m_GeneralMonsterErrorAction;
	m_CurrAction = &m_GeneralMonsterErrorAction;
	m_AfterAction = &m_GeneralMonsterErrorAction;
	ChangeStateToIdle();
}

void GeneralMonsterActionMgr::ChangeStateBefore()
{   
	if (m_BeforeAction == &m_IdleAction)
	{
		ChangeStateToIdle();
	}
	else if (m_BeforeAction == &m_MoveAction)
	{
		ChangeStateToMove();
	}
	else if (m_BeforeAction == &m_ChaseAction)
	{
		ChangeStateToChase();
	}
	else if (m_BeforeAction == &m_SearchAction)
	{
		ChangeStateToSearch();
	}
	else if (m_BeforeAction == &m_DeadAction)
	{
		ChangeStateToDead();
	}
	else if (m_BeforeAction == &m_HitAction)
	{
		ChangeStateToHit();
	}
	else if (m_BeforeAction == &m_AttackAction)
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
