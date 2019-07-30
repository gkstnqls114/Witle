#include "stdafx.h"
#include "GameObject.h"
#include "PlayerManager.h"
#include "Monster.h"
#include "Object.h"
#include "MonsterMovement.h"
#include "BossMonsterActionMgr.h"
#include "BossSkillAction0.h"


void BossSkillAction2::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}


void BossSkillAction2::UpdateState(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	// Attack �ִϸ��̼��� ������ �ʾ��� ��� Chnage State ���� �ʴ´�.
	if (!static_cast<Monster*>(m_pOwner)->GetpLoadObject()->IsTrackAnimationSetFinish(0, BOSS_SKILL2.ID)) return;

	actionMgr->ChangeBossStateToChase();
}