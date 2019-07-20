#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Monster.h"
#include "RecognitionRange.h"
#include "MonsterMovement.h"
#include "BossMonsterActionMgr.h"
#include "BossIdleAction.h"

void BossIdleAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	// 움직이지 않도록 고정
	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}

void BossIdleAction::UpdateState(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	Monster* pMonsterOwner = static_cast<Monster*>(m_pOwner);

	bool isNearPlayer = PlayerManager::IsNearPlayer(
		m_pOwner->GetTransform().GetPosition(),
		500
	);

	if (isNearPlayer)
	{
		actionMgr->ChangeStateToSample_2();
	}
}
