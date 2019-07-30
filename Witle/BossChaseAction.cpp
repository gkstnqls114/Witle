#include "stdafx.h"
#include "MonsterMovement.h"
#include "Player.h"
#include "BossMonsterActionMgr.h"
#include "PlayerManager.h"
#include "BossChaseAction.h"


void BossChaseAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
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

void BossChaseAction::UpdateState(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	int val = rand() % 5;

	if (PlayerManager::IsNearPlayer(m_pOwner->GetTransform().GetPosition(), 150))
	{
		if (val == 0)
		{
			(actionMgr)->ChangeBossStateToAction0();
		}
		else if (val == 1)
		{
			(actionMgr)->ChangeBossStateToAction1();
		}
		else if (val == 2)
		{
			(actionMgr)->ChangeBossStateToAction2();
		}
		else if (val == 3)
		{
			(actionMgr)->ChangeBossStateToAction3();
		}
		else if (val == 4)
		{
			(actionMgr)->ChangeBossStateToAction4();
		}
	}
}