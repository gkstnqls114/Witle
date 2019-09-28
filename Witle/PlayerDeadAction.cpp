#include "stdafx.h"

#include "SceneMgr.h"

#include "PlayerManager.h"
#include "Player.h"
#include "Monster.h"
#include "RecognitionRange.h"
#include "PlayerMovement.h"
#include "PlayerActionMgr.h"

#include "SoundManager.h"

#include "PlayerDeadAction.h"

void PlayerDeadAction::UpdateVelocity(float fElpasedTime, Movement * movement)
{
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_MOVE_SOUND);
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DAMAGE_SOUND);
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DASH_SOUND);
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_TAILATTACK_SOUND);
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_BREATH_SOUND);
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DEAD_SOUND);
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DOWNSTROKE_SOUND);

	// 움직이지 않도록 고정
	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}

void PlayerDeadAction::UpdateState(float fElpasedTime, PlayerActionMgr * actionMgr)
{
	if (static_cast<Player*>(m_pOwner)->IsTrackAnimationSetFinish(m_AnimationID))
	{
		SceneMgr::GetInstance()->ChangeSceneToLose();
		return;
	}

}
