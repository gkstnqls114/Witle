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
	static_cast<PlayerAction*>(m_CurrMonsterAction)->UpdateState(fElpasedTime, this);
}

void PlayerActionMgr::ChangeActionToIdle()
{
	bool isChanged = ChangeAction(&m_IdleAction);
	if (!isChanged) return;
	m_CurrMonsterAction->Init();
	m_PrevActionID = m_CurrActionID;
	m_CurrActionID = ANIMATION_IDLE.ID;
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_IDLE.ID);
	// static_cast<Player*>(m_pOwner)->SetisAttacking(false);
}

void PlayerActionMgr::ChangeActionToRightWalk()
{
	bool isChanged = ChangeAction(&m_RightWalkAction);
	if (!isChanged) return;
	m_CurrMonsterAction->Init();
	m_PrevActionID = m_CurrActionID;
	m_CurrActionID = ANIMATION_RIGHT.ID;
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_RIGHT.ID);
}

void PlayerActionMgr::ChangeActionToLeftWalk()
{
	bool isChanged = ChangeAction(&m_LeftWalkAction);
	if (!isChanged) return;
	m_CurrMonsterAction->Init();
	m_PrevActionID = m_CurrActionID;
	m_CurrActionID = ANIMATION_LEFT.ID;
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_LEFT.ID);
}

void PlayerActionMgr::ChangeActionToForwardWalk()
{
	bool isChanged = ChangeAction(&m_ForwardWalkAction);
	if (!isChanged) return;
	m_CurrMonsterAction->Init();
	m_PrevActionID = m_CurrActionID;
	m_CurrActionID = ANIMATION_FORWARD.ID;
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_FORWARD.ID);
}

void PlayerActionMgr::ChangeActionToBackwardWalk()
{
	bool isChanged = ChangeAction(&m_BackwardWalkAction);
	if (!isChanged) return;
	m_CurrMonsterAction->Init();
	m_PrevActionID = m_CurrActionID;
	m_CurrActionID = ANIMATION_BACKWARD.ID;
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_BACKWARD.ID);
}

void PlayerActionMgr::ChangeActionToStandardAttack()
{
	bool isChanged = ChangeAction(&m_StandardAttackAction);
	if (!isChanged) return;
	m_CurrMonsterAction->Init();
	m_PrevActionID = m_CurrActionID;
	m_CurrActionID = ANIMATION_ATTACK.ID;
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_ATTACK.ID);
}

void PlayerActionMgr::ChangeActionToBroomPrepare()
{
	bool isChanged = ChangeAction(&m_BroomPrepareAction);
	if (!isChanged) return;
	m_CurrMonsterAction->Init();
	m_PrevActionID = m_CurrActionID;
	m_CurrActionID = ANIMATION_BROOMPREPARE.ID;
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_BROOMPREPARE.ID);
}

void PlayerActionMgr::ChangeActionToBroomIdle()
{
	bool isChanged = ChangeAction(&m_BroomIdleAction);
	if (!isChanged) return;
	m_CurrMonsterAction->Init();
	m_PrevActionID = m_CurrActionID;
	m_CurrActionID = ANIMATION_BROOMIDLE.ID;
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_BROOMIDLE.ID);
}

void PlayerActionMgr::ChangeActionToBroomForward()
{
	bool isChanged = ChangeAction(&m_BroomForwardAction);
	if (!isChanged) return;
	m_CurrMonsterAction->Init();
	m_PrevActionID = m_CurrActionID;
	m_CurrActionID = ANIMATION_BROOMFORWARD.ID;
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_BROOMFORWARD.ID);
}

void PlayerActionMgr::ChangeActionToDead()
{
	bool isChanged = ChangeAction(&m_DeadAction);
	if (!isChanged) return;
	m_CurrMonsterAction->Init();
	m_PrevActionID = m_CurrActionID;
	m_CurrActionID = ANIMATION_DEAD.ID;
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_DEAD.ID); 
}

void PlayerActionMgr::ChangeActionToHit()
{
	bool isChanged = ChangeAction(&m_HitAction);
	if (!isChanged) return;
	m_CurrMonsterAction->Init();
	m_PrevActionID = m_CurrActionID;
	m_CurrActionID = ANIMATION_HIT.ID;
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_HIT.ID); 
}
 