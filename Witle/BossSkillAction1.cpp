#include "stdafx.h"
#include "GameObject.h"
#include "PlayerManager.h"
#include "Monster.h"
#include "Object.h"
#include "MonsterMovement.h"
#include "BossMonsterActionMgr.h"
#include "BossSkillAction0.h"

#include "SoundManager.h"

void BossSkillAction1::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	SoundManager::GetInstance()->Play(ENUM_SOUND::BOSS_DOWNSTROKE_SOUND);

	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}


void BossSkillAction1::UpdateState(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	// Attack 애니메이션이 끝나지 않았을 경우 Chnage State 하지 않는다.
	if (!static_cast<Monster*>(m_pOwner)->GetpLoadObject()->IsTrackAnimationSetFinish(0, BOSS_SKILL1.ID)) return;

	actionMgr->ChangeBossStateToChase();
}