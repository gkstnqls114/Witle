#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Monster.h"
#include "RecognitionRange.h"
#include "MonsterMovement.h"
#include "BossMonsterActionMgr.h"
#include "BossIdleAction.h"

void BossIdleAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	// �������� �ʵ��� ����
	movement->m_xmf3Velocity = XMFLOAT3(0.f, 0.f, 0.f);
}

void BossIdleAction::UpdateState(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	Monster* pMonsterOwner = static_cast<Monster*>(m_pOwner);

	bool isNearPlayer = PlayerManager::IsNearPlayer(m_pOwner->GetTransform().GetPosition(), 500);


	if (isNearPlayer)
	{
		actionMgr->SetZeroTotalTime(); // �̰� �� �Ѱ���.. ?

		pMonsterOwner->GetRecognitionRange()->m_TotalTime += fElpasedTime;

		// ���� �νĽð��� �Ǿ��� ���...
		if (pMonsterOwner->GetRecognitionRange()->m_TotalTime >= pMonsterOwner->GetRecognitionRange()->m_RecognitionTime)
		{
			pMonsterOwner->GetRecognitionRange()->m_TotalTime = 0;
			actionMgr->ChangeBossStateToChase();
		}
		return;
	}
	else
	{
		pMonsterOwner->GetRecognitionRange()->m_TotalTime = 0;
	}

	if (actionMgr->GetTotalTime() > m_IdleTime)
	{
		actionMgr->SetZeroTotalTime(); // �̰� �� �Ѱ���.. ?
		actionMgr->ChangeBossStateToMove();
	}
}
