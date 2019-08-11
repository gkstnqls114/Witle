#include "stdafx.h"
#include "GameObject.h"
#include "PlayerManager.h"
#include "Monster.h"
#include "Object.h"
#include "MonsterMovement.h"
#include "BossMonsterActionMgr.h"
#include "SoundManager.h"
#include "BossSkillDownStroke.h"


void BossSkillDownStroke::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	SoundManager::GetInstance()->Play(ENUM_SOUND::BOSS_DOWNSTROKE_SOUND);

	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}


void BossSkillDownStroke::UpdateState(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	// Attack �ִϸ��̼��� ������ �ʾ��� ��� Chnage State ���� �ʴ´�.
	if (!static_cast<Monster*>(m_pOwner)->GetpLoadObject()->IsTrackAnimationSetFinish(0, BOSS_DOWNSTORK.ID)) return;

	actionMgr->ChangeBossStateToChase();
}