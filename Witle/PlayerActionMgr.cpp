#include "stdafx.h"

#include "Object.h"
#include "PlayerMovement.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "PlayerAction.h"
#include "PlayerStatus.h"
#include "Player.h"

#include "PlayerActionMgr.h"
   
void PlayerActionMgr::UpdateState(float fElpasedTime)
{
	static_cast<PlayerAction*>(m_AfterAction)->UpdateState(fElpasedTime, this);
}

bool PlayerActionMgr::isDifferAction() const
{
	return ((static_cast<PlayerAction*>(m_BeforeAction)->m_ActionID) != (static_cast<PlayerAction*>(m_AfterAction)->m_ActionID));
}

bool PlayerActionMgr::isDifferAfterAndCurrent() const
{
	return ((static_cast<PlayerAction*>(m_AfterAction)->m_ActionID) != (static_cast<PlayerAction*>(m_CurrAction)->m_ActionID));
}

void PlayerActionMgr::Init()
{ 
	m_BeforeAction = &m_PlayerErrorAction;
	m_AfterAction = &m_IdleAction; 
	m_CurrAction = &m_PlayerErrorAction;
}

void PlayerActionMgr::ChangeActionToIdle()
{ 
	bool isChanged = ChangeAction(&m_IdleAction);
	if (!isChanged) return;
	m_AfterAction->Init(); 

	static_cast<Player*>(m_pOwner)->SetAnimationID(m_IdleAction.m_AnimationID);
	// static_cast<Player*>(m_pOwner)->SetisAttacking(false);
}

void PlayerActionMgr::ChangeActionToRightWalk()
{ 
	bool isChanged = ChangeAction(&m_RightWalkAction);
	if (!isChanged) return;
	m_AfterAction->Init(); 

	static_cast<Player*>(m_pOwner)->SetAnimationID(m_RightWalkAction.m_AnimationID);
}

void PlayerActionMgr::ChangeActionToLeftWalk()
{ 
	bool isChanged = ChangeAction(&m_LeftWalkAction);
	if (!isChanged) return;
	m_AfterAction->Init(); 

	static_cast<Player*>(m_pOwner)->SetAnimationID(m_LeftWalkAction.m_AnimationID);
}

void PlayerActionMgr::ChangeActionToForwardWalk()
{ 
	bool isChanged = ChangeAction(&m_ForwardWalkAction);
	if (!isChanged) return;
	m_AfterAction->Init(); 

	static_cast<Player*>(m_pOwner)->SetAnimationID(m_ForwardWalkAction.m_AnimationID);
}

void PlayerActionMgr::ChangeActionToBackwardWalk()
{ 
	bool isChanged = ChangeAction(&m_BackwardWalkAction);
	if (!isChanged) return;
	m_AfterAction->Init(); 

	static_cast<Player*>(m_pOwner)->SetAnimationID(m_BackwardWalkAction.m_AnimationID);
}

void PlayerActionMgr::ChangeActionToStandardAttack()
{ 
	bool isChanged = ChangeAction(&m_StandardAttackAction);
	if (!isChanged) return; 
	m_AfterAction->Init(); 

	static_cast<Player*>(m_pOwner)->SetAnimationID(m_StandardAttackAction.m_AnimationID);
}

void PlayerActionMgr::ChangeActionToBroomPrepare()
{ 
	bool isChanged = ChangeAction(&m_BroomPrepareAction);
	if (!isChanged) return; 
	m_AfterAction->Init(); 

	static_cast<Player*>(m_pOwner)->SetAnimationID(m_BroomPrepareAction.m_AnimationID);
}

void PlayerActionMgr::ChangeActionToBroomIdle()
{ 
	bool isChanged = ChangeAction(&m_BroomIdleAction);
	if (!isChanged) return;
	m_AfterAction->Init(); 

	static_cast<Player*>(m_pOwner)->SetAnimationID(m_BroomIdleAction.m_AnimationID);
}

void PlayerActionMgr::ChangeActionToBroomForward()
{ 
	bool isChanged = ChangeAction(&m_BroomForwardAction);
	if (!isChanged) return;
	m_AfterAction->Init(); 

	static_cast<Player*>(m_pOwner)->SetAnimationID(m_BroomForwardAction.m_AnimationID);
}

void PlayerActionMgr::ChangeActionToDead()
{ 
	bool isChanged = ChangeAction(&m_DeadAction);
	if (!isChanged) return;
	m_AfterAction->Init(); 

	static_cast<Player*>(m_pOwner)->SetAnimationID(m_DeadAction.m_AnimationID);
}

void PlayerActionMgr::ChangeActionToHit()
{
	bool isChanged = ChangeAction(&m_HitAction);
	if (!isChanged) return;
	m_AfterAction->Init(); 

	static_cast<Player*>(m_pOwner)->SetAnimationID(m_DeadAction.m_AnimationID);
}
 