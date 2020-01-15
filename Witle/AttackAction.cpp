#include "stdafx.h"
#include "GameObject.h"
#include "PlayerManager.h"
#include "Monster.h"
#include "Object.h"
#include "Movement.h"
#include "GeneralMonsterActionMgr.h"
#include "AttackAction.h"

void AttackAction::UpdateVelocity(float fElpasedTime, Movement * movement)
{
	movement->SetVelocity(XMFLOAT3(0.f, 0.f, 0.f));
}

void AttackAction::UpdateState(float fElpasedTime, GeneralMonsterActionMgr * actionMgr)
{
	// Attack 애니메이션이 끝나지 않았을 경우 Chnage State 하지 않는다.
	if (!static_cast<Monster*>(m_pOwner)->GetpLoadObject()->IsTrackAnimationSetFinish(0, SPACECAT_ATTACK.ID)) return;

	actionMgr->ChangeStateToChase();
}