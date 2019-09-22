#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Monster.h"
#include "RecognitionRange.h"
#include "PlayerMovement.h"
#include "PlayerActionMgr.h"

#include "SoundManager.h"

#include "PlayerHitAction.h"

void PlayerHitAction::UpdateVelocity(float fElpasedTime, Movement * movement)
{
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_MOVE_SOUND);
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DAMAGE_SOUND);
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DASH_SOUND);
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_TAILATTACK_SOUND);
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_BREATH_SOUND);
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DEAD_SOUND);
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DOWNSTROKE_SOUND);

	// �������� �ʵ��� ����
	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}

void PlayerHitAction::UpdateState(float fElpasedTime, PlayerActionMgr * actionMgr)
{
	m_HitElapsedTime += fElpasedTime;

	if (m_HitElapsedTime >= m_HitFullTime)
	{
		actionMgr->ChangeActionToIdle();
		m_HitElapsedTime = 0.f;
	}
}
