#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Monster.h"
#include "RecognitionRange.h"
#include "MonsterMovement.h"
#include "BossMonsterActionMgr.h"
#include "BossDead.h"
#include "SoundManager.h"

void BossDead::UpdateVelocity(float fElpasedTime, Movement * movement)
{
	// 움직이지 않도록 고정
	movement->SetVelocity( XMFLOAT3(0.f, 0.f, 0.f));

	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_MOVE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DAMAGE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DASH_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_TAILATTACK_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_BREATH_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::BOSS_DEAD_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DOWNSTROKE_SOUND);
}

void BossDead::UpdateState(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{ 
}
