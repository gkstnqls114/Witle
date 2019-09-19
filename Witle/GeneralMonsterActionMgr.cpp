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
	if (m_CurrAction != &m_DeadAction && static_cast<Monster*>(m_pOwner)->GetStatus()->GetGuage() <= 0)
	{
		ChangeStateToDead();
	}

	static_cast<GeneralMonsterAction*>(m_CurrAction)->UpdateState(fElpasedTime, this);

	// CHASE에서 IDLE로...
	//if (m_CurrAction == &m_ChaseAction
	//	&& !IsNearPlayer(PlayerManager::GetMainPlayer(), pMonsterOwner->GetRecognitionRange()->m_RecognitionRange))
	//{
	//	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_IDLE.ID); 
	//	m_TotalTime = 0.f;
	//	m_CurrAction = &m_IdleAction;
	//	return;
	//} 
}

void GeneralMonsterActionMgr::ChangeStateBefore()
{
	// 상태가 이전하고 다른 경우에만 서로 변경한다.
	if (m_CurrAction != m_BeforeAction)
	{
		Action* temp = m_CurrAction;
		m_CurrAction = m_BeforeAction;
		m_BeforeAction = temp;
	}

	if (m_CurrAction == &m_IdleAction)
	{
		ChangeStateToIdle();
	}
	else if (m_CurrAction == &m_MoveAction)
	{
		ChangeStateToMove();
	}
	else if (m_CurrAction == &m_ChaseAction)
	{
		ChangeStateToChase();
	}
	else if (m_CurrAction == &m_SearchAction)
	{
		ChangeStateToSearch();
	}
	else if (m_CurrAction == &m_DeadAction)
	{
		ChangeStateToDead();
	}
	else if (m_CurrAction == &m_HitAction)
	{
		ChangeStateToHit();
	}
	else if (m_CurrAction == &m_AttackAction)
	{
		ChangeStateToAttack();
	}
}

void GeneralMonsterActionMgr::ChangeStateToIdle()
{
	bool isChanged = ChangeAction(&m_IdleAction);
	if (!isChanged) return;
	m_CurrAction->Init();
	m_CurrActionID = SPACECAT_IDLE.ID;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_IDLE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void GeneralMonsterActionMgr::ChangeStateToMove()
{ 
	bool isChanged = ChangeAction(&m_MoveAction);
	if (!isChanged) return;
	m_CurrAction->Init();
	m_CurrActionID = SPACECAT_MOVE.ID;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void GeneralMonsterActionMgr::ChangeStateToChase()
{ 
	bool isChanged = ChangeAction(&m_ChaseAction);
	if (!isChanged) return;
	m_CurrAction->Init();
	m_CurrActionID = SPACECAT_MOVE.ID;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void GeneralMonsterActionMgr::ChangeStateToSearch()
{ 
	bool isChanged = ChangeAction(&m_SearchAction);
	if (!isChanged) return;
	m_CurrAction->Init();
	m_CurrActionID = SPACECAT_MOVE.ID;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void GeneralMonsterActionMgr::ChangeStateToDead()
{ 
	bool isChanged = ChangeAction(&m_DeadAction);
	if (!isChanged) return;
	m_CurrAction->Init();
	m_CurrActionID = SPACECAT_DEAD.ID;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_DEAD.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::SPACE_MOVE);
}

void GeneralMonsterActionMgr::ChangeStateToHit()
{ 
	bool isChanged = ChangeAction(&m_HitAction);
	if (!isChanged) return;
	m_CurrAction->Init();
	m_CurrActionID = SPACECAT_HIT.ID;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_HIT.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void GeneralMonsterActionMgr::ChangeStateToAttack()
{ 
	bool isChanged = ChangeAction(&m_AttackAction);
	if (!isChanged) return;
	m_CurrAction->Init();
	m_CurrActionID = SPACECAT_ATTACK.ID;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_ATTACK.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true); 
}
