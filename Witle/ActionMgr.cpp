#include "stdafx.h"

#include "Object.h"
#include "PlayerManager.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "MonsterStatus.h"
#include "Monster.h"
#include "Action.h"

#include "ActionMgr.h"
  

bool ActionMgr::ChangeAction(Action * action)
{ 
	if (!IsPossibleChangeAction(action)) return false;
	m_BeforeAction = m_AfterAction;
	m_AfterAction = action;
	return true;
}

bool ActionMgr::IsPossibleChangeAction(Action * action) const
{
	if (m_AfterAction == action) return false;
	return true;
}

void ActionMgr::UpdateVelocity(float fElpasedTime, Movement * movement)
{
	// ���� ����ȯ. �ݵ�� �����Ұ�.
	m_AfterAction->UpdateVelocity(fElpasedTime, movement);
}
