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
	// ���� ������ġ���� �����Ÿ� �־����� ���...
	Monster* pMonsterOwner = static_cast<Monster*>(m_pOwner);
	if (Vector3::Length(m_pOwner->GetTransform().GetPosition(), pMonsterOwner->GetSpawnPoint()) > pMonsterOwner->GetSpawnRange())
	{
		// ������ ���� ������ �ݴ�� �̵�
		SetDirection(Vector3::ScalarProduct(GetDirection(), -1, true));
	}
	else
	{
		// �����Ÿ����� �����Ÿ� �־������� �ƴ϶�� �׳� �������� ���ư���..
		SetDirection(Vector3::Random(false, true, false));
	}
}

void MoveAction::UpdateState(float fElpasedTime, GeneralMonsterActionMgr * actionMgr)
{
	Monster* pMonsterOwner = static_cast<Monster*>(m_pOwner);

	bool isNearPlayer = PlayerManager::IsNearPlayer(m_pOwner->GetTransform().GetPosition(), pMonsterOwner->GetRecognitionRange()->m_RecognitionRange);
	if (isNearPlayer)
	{
		actionMgr->SetZeroTotalTime(); // �̰� �� �Ѱ���.. ?

		pMonsterOwner->GetRecognitionRange()->m_TotalTime += fElpasedTime;

		// ���� �νĽð��� �Ǿ��� ���...
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
