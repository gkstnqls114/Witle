#include "stdafx.h"

#include "Object.h"
#include "PlayerManager.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "MonsterStatus.h"
#include "Monster.h"
#include "Action.h"

#include "ActionMgr.h"
  

bool ActionMgr::ChangeAction(Action * const action)
{ 
	if (isPossibleChangeAction(action))
	{
		m_AfterAction = action; 
		return true;
	}
	else
	{
		return false;
	}
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
	// ���� ����ȯ. �ݵ�� �����Ұ�.
	m_AfterAction->UpdateVelocity(fElpasedTime, movement);
}

void ActionMgr::SetUpPrevActionID()
{
	if (isAfterNoneAction()) return;
	if (!isDifferAfterAndCurrent()) return;

	// ���� ���¿� ������°� �ٸ� ��쿡�� ȣ���Ѵ�.
	if(m_BeforeAction != m_CurrAction) m_BeforeAction = m_CurrAction;
	m_CurrAction = m_AfterAction; 

	m_CurrAction->Init();
}
