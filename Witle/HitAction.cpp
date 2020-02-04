#include "stdafx.h"
#include "Monster.h"
#include "Object.h" 
#include "GeneralMonsterActionMgr.h"
#include "MonsterMovement.h"
#include "HitAction.h"
#include "SoundManager.h"

void HitAction::UpdateVelocity(float fElpasedTime, Movement * movement)
{ 
	movement->SetVelocity( XMFLOAT3(0.f, 0.f, 0.f));
}

void HitAction::UpdateState(float fElpasedTime, GeneralMonsterActionMgr * actionMgr)
{ 
	// 만약 몬스터의 히트 액션이 끝났다면...
	if (static_cast<Monster*>(m_pOwner)->GetpLoadObject()->IsTrackAnimationSetFinish(0, SPACECAT_HIT.ID))
	{
		actionMgr->ChangeStateBefore();
	} 
}
