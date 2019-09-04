#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Monster.h"
#include "RecognitionRange.h"
#include "PlayerMovement.h"
#include "PlayerActionMgr.h"

#include "SoundManager.h"

#include "PlayerRightWalkAction.h"

void PlayerRightWalkAction::UpdateVelocity(float fElpasedTime, PlayerMovement * movement)
{
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_MOVE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DAMAGE_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DASH_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_TAILATTACK_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_BREATH_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DEAD_SOUND);
	SoundManager::GetInstance()->Stop(ENUM_SOUND::BOSS_DOWNSTROKE_SOUND);

	// 움직이지 않도록 고정
	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}

void PlayerRightWalkAction::UpdateState(float fElpasedTime, PlayerActionMgr * actionMgr)
{
	Player* pMonsterOwner = static_cast<Player*>(m_pOwner);

	bool isNearPlayer = PlayerManager::IsNearPlayer(m_pOwner->GetTransform().GetPosition(), 500);


	//if (isNearPlayer)
	//{
	//	actionMgr->SetZeroTotalTime(); // 이거 왜 한거지.. ?

	//	pMonsterOwner->GetRecognitionRange()->m_TotalTime += fElpasedTime;

	//	// 만약 인식시간이 되었을 경우...
	//	if (pMonsterOwner->GetRecognitionRange()->m_TotalTime >= pMonsterOwner->GetRecognitionRange()->m_RecognitionTime)
	//	{
	//		pMonsterOwner->GetRecognitionRange()->m_TotalTime = 0;
	//		actionMgr->ChangeBossStateToChase();
	//	}
	//	return;
	//}
	//else
	//{
	//	pMonsterOwner->GetRecognitionRange()->m_TotalTime = 0;
	//}

	//if (actionMgr->GetTotalTime() > m_IdleTime)
	//{
	//	actionMgr->SetZeroTotalTime(); // 이거 왜 한거지.. ?
	//	actionMgr->ChangeBossStateToMove();
	//}
}
