#include "stdafx.h"

#include "Object.h"
#include "PlayerMovement.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "PlayerAction.h"
#include "PlayerStatus.h"
#include "Player.h"

#include "PlayerActionMgr.h"
  

void PlayerActionMgr::ChangeAction(PlayerAction * action)
{
	// ���� ���¿� �����ϴٸ� �ٲ��� �ʴ´�.
	if (m_CurrMonsterAction == action) return;

	m_BeforeMonsterAction = m_CurrMonsterAction;
	m_CurrMonsterAction = action;
}

void PlayerActionMgr::UpdateVelocity(float fElpasedTime, PlayerMovement * movement)
{
	m_CurrMonsterAction->UpdateVelocity(fElpasedTime, movement);
}
 