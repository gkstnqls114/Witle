#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Monster.h"
#include "RecognitionRange.h"
#include "PlayerMovement.h"
#include "PlayerActionMgr.h"
#include "Broom.h"

#include "SoundManager.h"

#include "PlayerBroomPrepareAction.h"

void PlayerBroomPrepareAction::UpdateVelocity(float fElpasedTime, Movement * movement)
{ 
	// 움직이지 않도록 고정
	movement->SetVelocity(XMFLOAT3(0.f, 0.f, 0.f));
}

void PlayerBroomPrepareAction::UpdateState(float fElpasedTime, PlayerActionMgr * actionMgr)
{
	if (static_cast<Player*>(m_pOwner)->IsTrackAnimationSetFinish(m_AnimationID))
	{ 
		actionMgr->ChangeActionToBroomIdle();
		static_cast<Player*>(m_pOwner)->GetBroom()->DoUse();
	}
}
