#include "stdafx.h"
#include "GameObject.h"
#include "PlayerManager.h"
#include "Monster.h"
#include "Object.h"
#include "MonsterMovement.h"
#include "BossMonsterActionMgr.h"
#include "BossSkillAction0.h"

#include "SoundManager.h"

void BossSkillAction0::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	SoundManager::GetInstance()->Play(ENUM_SOUND::BOSS_BREATH_SOUND);

	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}

void BossSkillAction0::UpdateState(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	// Attack �ִϸ��̼��� ������ �ʾ��� ��� Chnage State ���� �ʴ´�.
	if (!static_cast<Monster*>(m_pOwner)->GetpLoadObject()->IsTrackAnimationSetFinish(0, BOSS_SKILL0.ID)) return;

	actionMgr->ChangeBossStateToChase();
}