#include "stdafx.h"
#include "Monster.h"
#include "Object.h" 
#include "GeneralMonsterActionMgr.h"
#include "MonsterMovement.h"
#include "HitAction.h"
#include "SoundManager.h"

void HitAction::UpdateVelocity(float fElpasedTime, Movement * movement)
{
	// 사운드 ///////////////////////////////////////////////////////////
	// SoundManager::GetInstance()->Stop(ENUM_SOUND::MONSTER_MOVE_SOUND);
	// SoundManager::GetInstance()->Play(ENUM_SOUND::MONSTER_DAMAGE_SOUND);
	// 사운드 ///////////////////////////////////////////////////////////

	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}

void HitAction::UpdateState(float fElpasedTime, GeneralMonsterActionMgr * actionMgr)
{ 
	if (static_cast<Monster*>(m_pOwner)->GetpLoadObject()->IsTrackAnimationSetFinish(0, SPACECAT_HIT.ID))
	{
		actionMgr->ChangeStateBefore();
	} 
}
