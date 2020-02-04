#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Monster.h"
#include "RecognitionRange.h"
#include "PlayerMovement.h"
#include "PlayerActionMgr.h"

#include "SoundManager.h"

#include "PlayerStandardAttackAction.h"

void PlayerStandardAttackAction::UpdateVelocity(float fElpasedTime, Movement * movement)
{ 
	// 움직이지 않도록 고정
	movement->SetVelocity(XMFLOAT3(0.f, 0.f, 0.f));
}

void PlayerStandardAttackAction::UpdateState(float fElpasedTime, PlayerActionMgr * actionMgr)
{
	if (static_cast<Player*>(m_pOwner)->IsTrackAnimationSetFinish(m_AnimationID))
	{
		actionMgr->ChangeActionToIdle();
	}
}
