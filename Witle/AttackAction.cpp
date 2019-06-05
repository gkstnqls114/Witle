#include "stdafx.h"
#include "GameObject.h"
#include "PlayerManager.h"
#include "MonsterMovement.h"
#include "AttackAction.h"

void AttackAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}

void AttackAction::UpdateState(float fElpasedTime, MonsterActionMgr * actionMgr)
{
	if (PlayerManager::IsNearPlayer(m_pOwner->GetTransform().GetPosition(), 150))
	{
		actionMgr->ChangeStateToChase();
	}
}
