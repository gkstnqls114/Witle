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
	// ���� ���¿� �����ϴٸ� �ٲ��� �ʴ´�.
	if (m_CurrAction == action) return false;

	m_BeforeAction = m_CurrAction;
	m_CurrAction = action;
	return true;
}

void ActionMgr::UpdateVelocity(float fElpasedTime, Movement * movement)
{
	// ���� ����ȯ. �ݵ�� �����Ұ�.
	m_CurrAction->UpdateVelocity(fElpasedTime, movement);
}
 