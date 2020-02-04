#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Monster.h"
#include "RecognitionRange.h"
#include "PlayerMovement.h"
#include "PlayerActionMgr.h"

#include "SoundManager.h"

#include "PlayerHitAction.h"

void PlayerHitAction::UpdateVelocity(float fElpasedTime, Movement * movement)
{ 
	// 움직이지 않도록 고정
	movement->SetVelocity (XMFLOAT3(0.f, 0.f, 0.f));
}

void PlayerHitAction::UpdateState(float fElpasedTime, PlayerActionMgr * actionMgr)
{
	m_HitElapsedTime += fElpasedTime;

	if (m_HitElapsedTime >= m_HitFullTime)
	{
		actionMgr->ChangeActionToIdle();
		m_HitElapsedTime = 0.f;
	}
}
