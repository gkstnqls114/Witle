#include "stdafx.h"

#include "Object.h"
#include "PlayerManager.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "MonsterStatus.h"
#include "Monster.h"

#include "MonsterActionMgr.h"
  

void ActionMgr::ChangeAction(Action * action)
{
	// ���� ���¿� �����ϴٸ� �ٲ��� �ʴ´�.
	if (m_CurrMonsterAction == action) return;

	m_BeforeMonsterAction = m_CurrMonsterAction;
	m_CurrMonsterAction = action;
}

void ActionMgr::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	m_CurrMonsterAction->UpdateVelocity(fElpasedTime, movement);
}
 