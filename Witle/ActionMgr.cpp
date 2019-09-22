#include "stdafx.h"

#include "Object.h"
#include "PlayerManager.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "MonsterStatus.h"
#include "Monster.h"
#include "Action.h"

#include "ActionMgr.h"
  

bool ActionMgr::ChangeAfterAction(Action * const action)
{ 
	m_AfterAction = action; 
	return true;
}

void ActionMgr::ChangeImmediatelyAction(Action * const action)
{
	ChangeAfterAction(action);
	ProcessActions();
	SetTrackAnimationSet();
}

bool ActionMgr::isPossibleChangeAction(const Action * const action) const
{
	if (isSameAfterAction(action)) return false; 
	if (isSameCurrAction(action)) return false;
	return true;
}

bool ActionMgr::isSameAfterAction(const Action * const action) const
{
	return m_AfterAction == action;
}

bool ActionMgr::isSameCurrAction(const Action * const action) const
{
	return m_CurrAction == action;
}
 
void ActionMgr::UpdateVelocity(float fElpasedTime, Movement * movement)
{ 
	m_CurrAction->UpdateVelocity(fElpasedTime, movement);
}

void ActionMgr::ProcessActions()
{
	if (isAfterNoneAction()) return;
	if (!isDifferAfterAndCurrent()) return;

	// 이전 상태와 현재상태가 다른 경우에만 호출한다.
	if(m_BeforeAction != m_CurrAction) m_BeforeAction = m_CurrAction;

	m_CurrAction = m_AfterAction; 
	m_CurrAction->Init(); 
}
