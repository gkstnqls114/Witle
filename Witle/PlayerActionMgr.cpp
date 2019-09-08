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
	ChangeAction(&m_IdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_IDLE.ID);
	// static_cast<Player*>(m_pOwner)->SetisAttacking(false);
}

void PlayerActionMgr::ChangeActionToRightWalk()
{
	ChangeAction(&m_RightWalkAction);
	m_CurrMonsterAction->Init();
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_RIGHT.ID);
	// static_cast<Player*>(m_pOwner)->SetisAttacking(false);
}

void PlayerActionMgr::ChangeActionToLeftWalk()
{
	ChangeAction(&m_LeftWalkAction);
	m_CurrMonsterAction->Init();
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_LEFT.ID);
	// static_cast<Player*>(m_pOwner)->SetisAttacking(false);
}

void PlayerActionMgr::ChangeActionToForwardWalk()
{
	ChangeAction(&m_ForwardWalkAction);
	m_CurrMonsterAction->Init();
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_FORWARD.ID);
	// static_cast<Player*>(m_pOwner)->SetisAttacking(false);
}

void PlayerActionMgr::ChangeActionToBackwardWalk()
{
	ChangeAction(&m_BackwardWalkAction);
	m_CurrMonsterAction->Init();
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_BACKWARD.ID);
	// static_cast<Player*>(m_pOwner)->SetisAttacking(false);
}

void PlayerActionMgr::ChangeActionToStandardAttack()
{
	ChangeAction(&m_StandardAttackAction);
	m_CurrMonsterAction->Init();
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_ATTACK.ID);
	// static_cast<Player*>(m_pOwner)->SetisAttacking(false);
}

void PlayerActionMgr::ChangeActionToBroomPrepare()
{
	ChangeAction(&m_BroomPrepareAction);
	m_CurrMonsterAction->Init();
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_BROOMPREPARE.ID);
	// static_cast<Player*>(m_pOwner)->SetisAttacking(false);
}

void PlayerActionMgr::ChangeActionToBroomForward()
{
	ChangeAction(&m_BroomForwardAction);
	m_CurrMonsterAction->Init();
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_BROOMFORWARD.ID);
	// static_cast<Player*>(m_pOwner)->SetisAttacking(false);
}

void PlayerActionMgr::ChangeActionToDead()
{
	ChangeAction(&m_DeadAction);
	m_CurrMonsterAction->Init();
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_DEAD.ID);
	// static_cast<Player*>(m_pOwner)->SetisAttacking(false);
}

void PlayerActionMgr::ChangeActionToHit()
{
	ChangeAction(&m_HitAction);
	m_CurrMonsterAction->Init();
	static_cast<Player*>(m_pOwner)->SetAnimationState(ANIMATION_HIT.ID);
	// static_cast<Player*>(m_pOwner)->SetisAttacking(false);
}
 