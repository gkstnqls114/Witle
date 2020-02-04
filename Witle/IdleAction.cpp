#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Monster.h"
#include "RecognitionRange.h"
#include "MonsterMovement.h"
#include "GeneralMonsterActionMgr.h"
#include "IdleAction.h"

#include "SoundManager.h"

void IdleAction::UpdateVelocity(float fElpasedTime, Movement * movement)
{ 
	// 움직이지 않도록 고정
	movement->SetVelocity( XMFLOAT3(0.f, 0.f, 0.f));
}

void IdleAction::UpdateState(float fElpasedTime, GeneralMonsterActionMgr * actionMgr)
{
	Monster* pMonsterOwner = static_cast<Monster*>(m_pOwner);

	bool isNearPlayer = PlayerManager::IsNearPlayer(m_pOwner->GetTransform().GetPosition(), pMonsterOwner->GetRecognitionRange()->m_RecognitionRange);
	if (isNearPlayer)
	{
		actionMgr->SetZeroTotalTime();

		pMonsterOwner->GetRecognitionRange()->m_TotalTime += fElpasedTime;

		// 만약 인식시간이 되었을 경우...
		if (pMonsterOwner->GetRecognitionRange()->m_TotalTime >= pMonsterOwner->GetRecognitionRange()->m_RecognitionTime)
		{
			pMonsterOwner->GetRecognitionRange()->m_TotalTime = 0;
			actionMgr->ChangeStateToChase();
		}
		return;
	}
	else
	{
		pMonsterOwner->GetRecognitionRange()->m_TotalTime = 0;
	}


	if (actionMgr->GetTotalTime() > m_IdleTime)
	{
		actionMgr->SetZeroTotalTime(); 
		actionMgr->ChangeStateToMove();
	}
}
