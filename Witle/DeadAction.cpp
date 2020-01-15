#include "stdafx.h"
#include "MonsterMovement.h"
#include "DeadAction.h"

#include "SoundManager.h"

void DeadAction::UpdateVelocity(float fElpasedTime, Movement * movement)
{
	SoundManager::GetInstance()->Stop(ENUM_SOUND::MONSTER_DAMAGE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::MONSTER_MOVE_SOUND);
	SoundManager::GetInstance()->Play(ENUM_SOUND::MONSTER_DEAD_SOUND);

	movement->SetVelocity(XMFLOAT3(0.f, 0.f, 0.f));
}

void DeadAction::UpdateState(float fElpasedTime, GeneralMonsterActionMgr * actionMgr)
{
	// 죽은 이후에는 상태를 업데이트할 필요가 없습니다.
}
