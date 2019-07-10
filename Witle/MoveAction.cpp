#include "stdafx.h"
#include "Monster.h"
#include "Player.h"
#include "PlayerManager.h" 
#include "RecognitionRange.h"
#include "MonsterMovement.h"
#include "GeneralMonsterActionMgr.h"

#include "MoveAction.h"

#include "SoundManager.h"

void MoveAction::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	movement->m_xmf3Velocity = Vector3::ScalarProduct(m_Direction, movement->m_fDistance, false);
	SoundManager::GetInstance()->Play(ENUM_SOUND::MUSHROOM_MOVE);

	XMFLOAT3 look(0.f, 0.f, 1.f);
	float value = Vector3::AngleToRadian(look, m_Direction) * 180.f / 3.141592;
	if (Vector3::CrossProduct(look, m_Direction).y >= 0)
	{
		movement->m_fPitch = value;
	}
	else
	{
		movement->m_fPitch = -value;
	}

}

void MoveAction::Init()
{
	// 만약 스폰위치에서 일정거리 멀어졌을 경우...
	Monster* pMonsterOwner = static_cast<Monster*>(m_pOwner);
	if (Vector3::Length(m_pOwner->GetTransform().GetPosition(), pMonsterOwner->GetSpawnPoint()) > pMonsterOwner->GetSpawnRange())
	{
		// 이전에 갔던 방향을 반대로 이동
		SetDirection(Vector3::ScalarProduct(GetDirection(), -1, true));
	}
	else
	{
		// 스폰거리에서 일정거리 멀어진것이 아니라면 그냥 랜덤으로 돌아간다..
		SetDirection(Vector3::Random(false, true, false));
	}
}

void MoveAction::UpdateState(float fElpasedTime, GeneralMonsterActionMgr * actionMgr)
{
	Monster* pMonsterOwner = static_cast<Monster*>(m_pOwner);

	bool isNearPlayer = PlayerManager::IsNearPlayer(m_pOwner->GetTransform().GetPosition(), pMonsterOwner->GetRecognitionRange()->m_RecognitionRange);
	if (isNearPlayer)
	{
		actionMgr->SetZeroTotalTime(); // 이거 왜 한거지.. ?

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


	if (actionMgr->GetTotalTime() > m_MoveTime)
	{
		actionMgr->SetZeroTotalTime();
		actionMgr->ChangeStateToIdle();
	}
}
