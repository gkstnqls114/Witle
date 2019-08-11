#include "stdafx.h"
#include "GameObject.h"
#include "PlayerManager.h"
#include "Monster.h"
#include "Object.h"
#include "MonsterMovement.h"
#include "BossMonsterActionMgr.h"
#include "Skill.h"
#include "BossSkillMgr.h"
#include "Dragon.h"

#include "SoundManager.h"

#include "BossSkillBreath.h"

void BossSkillBreath::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	SoundManager::GetInstance()->Play(ENUM_SOUND::BOSS_BREATH_SOUND);

	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}

void BossSkillBreath::UpdateState(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	for (int x = 0; x < 10; ++x)
	{ 
		if (BossSkillMgr::GetInstance()->GetpSelectableSkill(x)->isActive == true) continue;

		BossSkillMgr::GetInstance()->Activate(static_cast<Dragon*>(m_pOwner), x);
		break;
	}

	// Attack 애니메이션이 끝나지 않았을 경우 Chnage State 하지 않는다.
	if (!static_cast<Monster*>(m_pOwner)->GetpLoadObject()->IsTrackAnimationSetFinish(0, BOSS_BREATH.ID)) return;

	actionMgr->ChangeBossStateToChase(); 
}