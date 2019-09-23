#include "stdafx.h"

#include "Broom.h"
#include "Object.h"
#include "PlayerMovement.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "PlayerAction.h"
#include "PlayerStatus.h"
#include "Player.h"
#include "GameInput.h"

#include "PlayerActionMgr.h"
   
void PlayerActionMgr::UpdateState(float fElpasedTime)
{
	if (static_cast<Player*>(m_pOwner)->GetHPStatus()->GetGuage() <= 0 && !m_isDead)
	{
		// 곧바로 Dead 상태로 전환
		ChangeActionToDead();
		m_isDead = true; 
	}

	if (isRunMode())
	{ 
		// WASD 아무것도 누르지 않는 경우
		if (!GameInput::IsKeydownW() && !GameInput::IsKeydownS() && !GameInput::IsKeydownA() && !GameInput::IsKeydownD())
		{
			ChangeActionToIdle();
		} 
		if (GameInput::IsKeydownW())
		{
			ChangeActionToForwardWalk(); 
		}
		if (GameInput::IsKeydownS())
		{
			ChangeActionToBackwardWalk(); 
		}
		if (GameInput::IsKeydownA())
		{
			ChangeActionToLeftWalk(); 
		}
		if (GameInput::IsKeydownD())
		{
			ChangeActionToRightWalk(); 
		} 

	}
	else if (isBroomMode())
	{
		// WASD 아무것도 누르지 않는 경우
		if (!GameInput::IsKeydownW() && !GameInput::IsKeydownS() && !GameInput::IsKeydownA() && !GameInput::IsKeydownD())
		{
			ChangeActionToBroomIdle();
		}
		else
		{ 
			ChangeActionToBroomForward();
		} 

		if (!static_cast<Player*>(m_pOwner)->GetBroom()->GetisUsing())
		{ 
			ChangeActionToIdle();
		}
	}
	else
	{
 		static_cast<PlayerAction*>(m_CurrAction)->UpdateState(fElpasedTime, this);
	}
}
 
bool PlayerActionMgr::isDifferAfterAndCurrent() const
{  
	if ((static_cast<PlayerAction*>(m_AfterAction)->m_ActionID == ENUM_PLAYER_ACTIONID::PLAYER_ACTIONID_ERROR) && (static_cast<PlayerAction*>(m_CurrAction)->m_ActionID == ENUM_PLAYER_ACTIONID::PLAYER_ACTIONID_ERROR)) return true;
	if ((static_cast<PlayerAction*>(m_AfterAction)->m_ActionID) != (static_cast<PlayerAction*>(m_CurrAction)->m_ActionID)) return true;
	else return false;
}

void PlayerActionMgr::SetUpAfterAction()
{
	m_AfterAction = &m_PlayerErrorAction; 
}

bool PlayerActionMgr::isAfterNoneAction()
{
	return isSameAfterAction(&m_PlayerErrorAction);
}

void PlayerActionMgr::SetTrackAnimationSet() const
{
	static_cast<Player*>(m_pOwner)->SetTrackAnimationSet(m_CurrAction->m_AnimationID);
}

void PlayerActionMgr::Init()
{ 
	m_isDead = false;
	m_BeforeAction = &m_PlayerErrorAction;
	m_AfterAction = &m_PlayerErrorAction;
	m_CurrAction = &m_PlayerErrorAction;
	ChangeActionToIdle();
}

bool PlayerActionMgr::isRunMode() const
{
	return Is_IdleAction() || Is_RightWalkAction() || Is_LeftWalkAction() || Is_BackwardWalkAction() || Is_ForwardWalkAction();
}

bool PlayerActionMgr::isBroomMode() const
{
	return Is_BroomIdleAction() || Is_BroomForwardAction();
}

PlayerActionMgr::PlayerActionMgr(Player * pOwner)
	: ActionMgr(static_cast<GameObject*>(pOwner))
	, m_PlayerErrorAction(pOwner)
	, m_IdleAction(pOwner, 0)
	, m_RightWalkAction(pOwner, 0)
	, m_LeftWalkAction(pOwner, 0)
	, m_ForwardWalkAction(pOwner, 0)
	, m_BackwardWalkAction(pOwner, 0)
	, m_StandardAttackAction(pOwner, 0)
	, m_BroomPrepareAction(pOwner, 0)
	, m_BroomIdleAction(pOwner, 0)
	, m_BroomForwardAction(pOwner, 0)
	, m_DeadAction(pOwner, 0)
	, m_HitAction(pOwner)
{
	m_BeforeAction = &m_PlayerErrorAction;
	m_AfterAction = &m_PlayerErrorAction;
	m_CurrAction = &m_PlayerErrorAction;
}

void PlayerActionMgr::UpdateVelocity(float fElpasedTime, Movement * const movement)
{
	// 빗자루를 사용하지않고 서있거나, 상하좌우 이동하는 경우
	if (isRunMode() || isBroomMode())
	{
		DWORD dwDirection = 0;
		 
		if (GameInput::IsKeydownW())
		{ 
			dwDirection |= DIR_FORWARD;
		}
		if (GameInput::IsKeydownS())
		{ 
			dwDirection |= DIR_BACKWARD;
		}
		if (GameInput::IsKeydownA())
		{ 
			dwDirection |= DIR_LEFT;
		}
		if (GameInput::IsKeydownD())
		{ 
			dwDirection |= DIR_RIGHT;
		}

		// 만약 키보드 상하좌우 움직인다면...
		if (dwDirection != 0)
		{
			//플레이어의 이동량 벡터를 xmf3Shift 벡터만큼 더한다. 
			static_cast<PlayerMovement*>(movement)->MoveVelocity(dwDirection, fElpasedTime);
		}
		else
		{
			static_cast<PlayerMovement*>(movement)->ReduceVelocity(fElpasedTime);
		}
	} 
	else
	{
		m_CurrAction->UpdateVelocity(fElpasedTime, movement);
	}
}

void PlayerActionMgr::ChangeActionToIdle()
{
	bool isChanged = ChangeAfterAction(&m_IdleAction);
	if (!isChanged) return;
	m_AfterAction->Init(); 

}

void PlayerActionMgr::ChangeActionToRightWalk()
{ 
	bool isChanged = ChangeAfterAction(&m_RightWalkAction);
	if (!isChanged) return;

}

void PlayerActionMgr::ChangeActionToLeftWalk()
{ 
	bool isChanged = ChangeAfterAction(&m_LeftWalkAction);
	if (!isChanged) return;
}

void PlayerActionMgr::ChangeActionToForwardWalk()
{ 
	bool isChanged = ChangeAfterAction(&m_ForwardWalkAction);
	if (!isChanged) return;

}

void PlayerActionMgr::ChangeActionToBackwardWalk()
{ 
	bool isChanged = ChangeAfterAction(&m_BackwardWalkAction);
	if (!isChanged) return;

}

void PlayerActionMgr::ChangeActionToStandardAttack()
{ 
	// 빗자루 모드일 경우 액션 변경하지 않는다.
	if (isBroomMode() || Is_BroomPrepareAction()) return;

	ChangeImmediatelyAction(&m_StandardAttackAction);
}

void PlayerActionMgr::ChangeActionToBroomPrepare()
{ 
	// 빗자루 모드일 경우 액션 변경하지 않는다.
	if (isBroomMode() || Is_BroomPrepareAction()) return;

	ChangeImmediatelyAction(&m_BroomPrepareAction);
}

void PlayerActionMgr::ChangeActionToBroomIdle()
{ 
	ChangeAfterAction(&m_BroomIdleAction);
}

void PlayerActionMgr::ChangeActionToBroomForward()
{ 
	ChangeAfterAction(&m_BroomForwardAction);
}

void PlayerActionMgr::ChangeActionToDead()
{ 
	ChangeImmediatelyAction(&m_DeadAction);
}

void PlayerActionMgr::ChangeActionToHit()
{
	// 빗자루 모드일 경우 액션 변경하지 않는다.
	if (isBroomMode() || Is_BroomPrepareAction()) return;

	ChangeImmediatelyAction(&m_HitAction);
}
 