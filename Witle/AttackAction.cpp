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
	// Attack �ִϸ��̼��� ������ �ʾ��� ��� Chnage State ���� �ʴ´�.
	if (!static_cast<Monster*>(m_pOwner)->GetpLoadObject()->IsTrackAnimationSetFinish(0, SPACECAT_ATTACK.ID)) return;

	actionMgr->ChangeStateToChase();
}