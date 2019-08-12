#include "stdafx.h"
#include "MonsterMovement.h"
#include "Player.h"
#include "GeneralMonsterActionMgr.h"
#include "PlayerManager.h"
#include "ChaseAction.h"

#include "SoundManager.h"

void ChaseAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	// 사운드 ///////////////////////////////////////////////////////////
	SoundManager::GetInstance()->Play(ENUM_SOUND::MONSTER_MOVE_SOUND);
	// 사운드 ///////////////////////////////////////////////////////////

	XMFLOAT3 toPlayer = Vector3::Normalize(
		Vector3::Subtract(PlayerManager::GetMainPlayer()->GetTransform().GetPosition(), m_pOwner->GetTransform().GetPosition())
	);

	// chase 인 경우 2배 빠른 이동
	movement->m_xmf3Velocity = Vector3::ScalarProduct(toPlayer, movement->m_fDistance * 1.7f, false);

	XMFLOAT3 look(0.f, 0.f, 1.f);
	float value = Vector3::AngleToRadian(look, toPlayer) * 180.f / 3.141592;
	if (Vector3::CrossProduct(look, toPlayer).y >= 0)
	{
		movement->m_fPitch = value;

		for (int i = 0; i < 5; i++)
		{
			// pChannel[(int)ENUM_SOUND::MONSTER_MOVE_SOUND]->setVolume(i);
		}
	}
	else
	{
		movement->m_fPitch = -value;

		for (int i = 0; i < 5; i--)
		{
			// pChannel[(int)ENUM_SOUND::MONSTER_MOVE_SOUND]->setVolume(i);
		}
	}
}

void ChaseAction::UpdateState(float fElpasedTime, GeneralMonsterActionMgr * actionMgr)
{
	if (PlayerManager::IsNearPlayer(m_pOwner->GetTransform().GetPosition(), 150))
	{
		(actionMgr)->ChangeStateToAttack();
	}
}
