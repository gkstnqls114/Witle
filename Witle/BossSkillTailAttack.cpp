#include "stdafx.h"
#include "GameObject.h"
#include "PlayerManager.h"
#include "Monster.h"
#include "Object.h"
#include "MonsterMovement.h"
#include "BossMonsterActionMgr.h"
#include "BossSkillTailAttack.h"

#include "SoundManager.h"

void BossSkillTailAttack::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_MOVE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DAMAGE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DASH_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::BOSS_TAILATTACK_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_BREATH_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DEAD_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DOWNSTROKE_SOUND);

	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}


void BossSkillTailAttack::UpdateState(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	// Attack �ִϸ��̼��� ������ �ʾ��� ��� Chnage State ���� �ʴ´�.
	if (!static_cast<Monster*>(m_pOwner)->GetpLoadObject()->IsTrackAnimationSetFinish(0, BOSS_TAILATTACK.ID)) return;

	actionMgr->ChangeBossStateToChase();
}