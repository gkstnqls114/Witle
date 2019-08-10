#include "stdafx.h"
#include "GameObject.h"
#include "PlayerManager.h"
#include "Monster.h"
#include "Object.h"
#include "MonsterMovement.h"
#include "BossMonsterActionMgr.h"
#include "BossSkillTailAttack.h"


void BossSkillTailAttack::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}


void BossSkillTailAttack::UpdateState(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	// Attack 애니메이션이 끝나지 않았을 경우 Chnage State 하지 않는다.
	if (!static_cast<Monster*>(m_pOwner)->GetpLoadObject()->IsTrackAnimationSetFinish(0, BOSS_TAILATTACK.ID)) return;

	actionMgr->ChangeBossStateToChase();
}