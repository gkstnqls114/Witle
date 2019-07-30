#include "stdafx.h"
#include "MonsterMovement.h"
#include "Player.h"
#include "GeneralMonsterActionMgr.h"
#include "PlayerManager.h"
#include "ChaseAction.h"

void ChaseAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	XMFLOAT3 toPlayer = Vector3::Normalize(
		Vector3::Subtract(PlayerManager::GetMainPlayer()->GetTransform().GetPosition(), m_pOwner->GetTransform().GetPosition())
	);

	movement->m_xmf3Velocity = Vector3::ScalarProduct(toPlayer, movement->m_fDistance, false);

	XMFLOAT3 look(0.f, 0.f, 1.f);
	float value = Vector3::AngleToRadian(look, toPlayer) * 180.f / 3.141592;
	if (Vector3::CrossProduct(look, toPlayer).y >= 0)
	{
		movement->m_fPitch = value;
	}
	else
	{
		movement->m_fPitch = -value;
	}
}

void ChaseAction::UpdateState(float fElpasedTime, GeneralMonsterActionMgr * actionMgr)
{
	if (PlayerManager::IsNearPlayer(m_pOwner->GetTransform().GetPosition(), 150))
	{
		(actionMgr)->ChangeStateToAttack();
	}
}
