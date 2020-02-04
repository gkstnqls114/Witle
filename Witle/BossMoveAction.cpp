#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Monster.h"
#include "RecognitionRange.h"
#include "MonsterMovement.h"
#include "BossMonsterActionMgr.h"
#include "BossMoveAction.h"

#include "SoundManager.h"

void BossMoveAction::UpdateVelocity(float fElpasedTime, Movement * movement)
{
	XMFLOAT3 toPlayer = Vector3::Normalize(
		Vector3::Subtract(
			PlayerManager::GetMainPlayer()->GetTransform().GetPosition(),
			m_pOwner->GetTransform().GetPosition()
		)
	);

	movement->SetVelocity(
		Vector3::ScalarProduct(toPlayer, movement->m_fDistance, false));

	XMFLOAT3 look(0.f, 0.f, 1.f);
	float value = Vector3::AngleToRadian(look, toPlayer) * 180.f / 3.141592;
	if (Vector3::CrossProduct(look, toPlayer).y >= 0)
	{
		movement->m_fPitch = value;
	}
	else
	{
		movement->m_fPitch = -value;
	}
}

void BossMoveAction::UpdateState(float fElpasedTime, BossMonsterActionMgr * actionMgr)
{
	Monster* pMonsterOwner = static_cast<Monster*>(m_pOwner);

	bool isNearPlayer = PlayerManager::IsNearPlayer(m_pOwner->GetTransform().GetPosition(), 500);

	if (!isNearPlayer)
	{
		actionMgr->SetZeroTotalTime();

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

	if (actionMgr->GetTotalTime() > m_MoveTime)
	{
		actionMgr->SetZeroTotalTime();
		actionMgr->ChangeBossStateToIdle();
	}
}
