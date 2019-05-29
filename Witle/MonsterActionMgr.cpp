#include "stdafx.h"

#include "PlayerManager.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "Monster.h"

#include "MonsterActionMgr.h"
 
bool MonsterActionMgr::IsNearPlayer(const Player * target, float distance)
{
	return Vector3::Length(Vector3::Subtract(m_pOwner->GetTransform().GetPosition(), target->GetTransform().GetPosition())) < distance;
}

void MonsterActionMgr::UpdateVelocity(float fElpasedTime, MonsterMovement * movement)
{
	m_CurrMonsterAction->UpdateVelocity(fElpasedTime, movement);
}

void MonsterActionMgr::UpdateState(float fElpasedTime)
{ 
	Monster* pMonsterOwner = static_cast<Monster*>(m_pOwner);

	// 어떤 상황이던간에(chase가 아닌경우) 인식 근처에 플레이어가 있는 경우 따라간다.
	if (m_CurrMonsterAction != &m_ChaseAction &&
		IsNearPlayer(PlayerManager::GetMainPlayer(), pMonsterOwner->GetRecognitionRange()->m_RecognitionRange))
	{
		m_TotalTime = 0.f;

		pMonsterOwner->GetRecognitionRange()->m_TotalTime += fElpasedTime;

		// 만약 인식시간이 되었을 경우...
		if (pMonsterOwner->GetRecognitionRange()->m_TotalTime >= pMonsterOwner->GetRecognitionRange()->m_RecognitionTime)
		{
			pMonsterOwner->GetRecognitionRange()->m_TotalTime = 0;
			m_CurrMonsterAction = &m_ChaseAction;
		}
		return;
	}
	else if ((m_CurrMonsterAction != &m_ChaseAction &&
		!IsNearPlayer(PlayerManager::GetMainPlayer(), pMonsterOwner->GetRecognitionRange()->m_RecognitionRange)))
	{
		pMonsterOwner->GetRecognitionRange()->m_TotalTime = 0;
	}

	if (m_CurrMonsterAction == &m_ChaseAction
		&& !IsNearPlayer(PlayerManager::GetMainPlayer(), pMonsterOwner->GetRecognitionRange()->m_RecognitionRange))
	{
		m_TotalTime = 0.f;
		m_CurrMonsterAction = &m_IdleAction;
		return;
	}

	if (m_CurrMonsterAction == &m_ChaseAction) return;

	m_TotalTime += fElpasedTime;
	if (m_CurrMonsterAction == &m_IdleAction)
	{
		if (m_TotalTime > m_IdleTime)
		{
			m_TotalTime = 0.f;
			m_CurrMonsterAction = &m_MoveAction;

			MoveAction* pMoveAction = static_cast<MoveAction*>(m_CurrMonsterAction);
			// 만약 스폰위치에서 일정거리 멀어졌을 경우...
			if (Vector3::Length(m_pOwner->GetTransform().GetPosition(), pMonsterOwner->GetSpawnPoint()) > pMonsterOwner->GetSpawnRange())
			{
				// 이전에 갔던 방향을 반대로 이동
				pMoveAction->SetDirection(
					Vector3::ScalarProduct(pMoveAction->GetDirection(), -1, false)
				);
			}
			else
			{
				// 스폰거리에서 일정거리 멀어진것이 아니라면 그냥 랜덤으로 돌아간다..
				static_cast<MoveAction*>(m_CurrMonsterAction)->SetDirection(XMFLOAT3(50 + rand() % 100, 0, 50 + rand() % 100));
			}
			return;
		}
	}

	if (m_CurrMonsterAction == &m_MoveAction)
	{
		if (m_TotalTime > m_MoveTime)
		{
			m_TotalTime = 0.f;
			m_CurrMonsterAction = &m_IdleAction;
			return;
		}
	} 
}

void MonsterActionMgr::ChangeStateToIdle()
{
}

void MonsterActionMgr::ChangeStateToMove()
{
}

void MonsterActionMgr::ChangeStateToChase()
{
}

void MonsterActionMgr::ChangeStateToSearch()
{
}

void MonsterActionMgr::ChangeStateToDead()
{
}

void MonsterActionMgr::ChangeStateToHit()
{
}
