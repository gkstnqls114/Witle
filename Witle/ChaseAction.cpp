#include "stdafx.h"
#include "MonsterMovement.h"
#include "Player.h"
#include "GeneralMonsterActionMgr.h"
#include "PlayerManager.h"
#include "ChaseAction.h"
#include "Monster.h"
#include "RecognitionRange.h"

#include "SoundManager.h"

void ChaseAction::UpdateVelocity(float fElpasedTime, Movement * movement)
{ 
	// 플레이어를 향한 정규화된 방향 벡터
	XMFLOAT3 toPlayer = Vector3::Normalize(
		Vector3::Subtract(PlayerManager::GetMainPlayer()->GetTransform().GetPosition(), m_pOwner->GetTransform().GetPosition())
	);

	// chase 인 경우 약 1.7배 빠른 이동
	movement->SetVelocity (
		Vector3::ScalarProduct(toPlayer, movement->m_fDistance * 1.7f, false));

	// 오른쪽 회전과 왼쪽 회전 구분을 위해 계산.
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

void ChaseAction::UpdateState(float fElpasedTime, GeneralMonsterActionMgr * actionMgr)
{
	Monster* pMonsterOwner = static_cast<Monster*>(m_pOwner);
	bool isOutRecognitionRange = !PlayerManager::IsNearPlayer(m_pOwner->GetTransform().GetPosition(), pMonsterOwner->GetRecognitionRange()->m_RecognitionRange);

	if (PlayerManager::IsNearPlayer(m_pOwner->GetTransform().GetPosition(), 150))
	{
		m_CognizeTime = 0.f;
		(actionMgr)->ChangeStateToAttack();
	}
	else if (isOutRecognitionRange)
	{
		m_CognizeTime += fElpasedTime;
		if (m_CognizeTime > m_MaxCognizeTime)
		{
			m_CognizeTime = 0.f;
			(actionMgr)->ChangeStateToMove();
		}
	}
}
