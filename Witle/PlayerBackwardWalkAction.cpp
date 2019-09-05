#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Monster.h"
#include "RecognitionRange.h"
#include "PlayerMovement.h"
#include "PlayerActionMgr.h"

#include "SoundManager.h"

#include "PlayerBackwardWalkAction.h"

void PlayerBackwardWalkAction::UpdateVelocity(float fElpasedTime, Movement * movement)
{

}

void PlayerBackwardWalkAction::UpdateState(float fElpasedTime, PlayerActionMgr * actionMgr)
{
	Player* pMonsterOwner = static_cast<Player*>(m_pOwner);

	bool isNearPlayer = PlayerManager::IsNearPlayer(m_pOwner->GetTransform().GetPosition(), 500);


	//if (isNearPlayer)
	//{
	//	actionMgr->SetZeroTotalTime(); // �̰� �� �Ѱ���.. ?

	//	pMonsterOwner->GetRecognitionRange()->m_TotalTime += fElpasedTime;

	//	// ���� �νĽð��� �Ǿ��� ���...
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
	//	actionMgr->SetZeroTotalTime(); // �̰� �� �Ѱ���.. ?
	//	actionMgr->ChangeBossStateToMove();
	//}
}
