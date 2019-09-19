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
	// 현재 상태와 동일하다면 바꾸지 않는다.
	if (m_CurrAction == action) return false;

	m_BeforeAction = m_CurrAction;
	m_CurrAction = action;
	return true;
}

void ActionMgr::UpdateVelocity(float fElpasedTime, Movement * movement)
{
	// 강제 형변환. 반드시 수정할것.
	m_CurrAction->UpdateVelocity(fElpasedTime, movement);
}
 