#include "stdafx.h"

#include "Object.h"
#include "PlayerManager.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "MonsterStatus.h"
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
	if (m_CurrMonsterAction == &m_DeadAction) return;

	if (static_cast<Monster*>(m_pOwner)->GetStatus()->m_HP <= 0)
	{
		ChangeStateToDead();
	}


	if (m_CurrMonsterAction == &m_HitAction)
	{
		if (static_cast<Monster*>(m_pOwner)->GetpLoadObject()->IsTrackAnimationSetFinish(0, SPACECAT_HIT.ID))
		{ 
			MonsterAction* temp = m_CurrMonsterAction;
			m_CurrMonsterAction = m_BeforeMonsterAction;
			m_BeforeMonsterAction = temp;

			if (m_CurrMonsterAction == &m_ChaseAction || m_CurrMonsterAction == &m_MoveAction)
			{ 
				static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
			}
			else
			{ 
				static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_IDLE.ID);
			}
		}
		return;
	}

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
			static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
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

	// CHASE에서 IDLE로...
	//if (m_CurrMonsterAction == &m_ChaseAction
	//	&& !IsNearPlayer(PlayerManager::GetMainPlayer(), pMonsterOwner->GetRecognitionRange()->m_RecognitionRange))
	//{
	//	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_IDLE.ID); 
	//	m_TotalTime = 0.f;
	//	m_CurrMonsterAction = &m_IdleAction;
	//	return;
	//}

	if (m_CurrMonsterAction == &m_ChaseAction)
	{
		if (IsNearPlayer(PlayerManager::GetMainPlayer(), 150))
		{
			ChangeStateToAttack();
		} 
		return;
	}
	if (m_CurrMonsterAction == &m_AttackAction)
	{ 
		if (!IsNearPlayer(PlayerManager::GetMainPlayer(), 150))
		{
			ChangeStateToChase();
		}
		return; 
	}

	m_TotalTime += fElpasedTime;
	if (m_CurrMonsterAction == &m_IdleAction)
	{
		if (m_TotalTime > m_IdleTime)
		{
			m_TotalTime = 0.f;
			static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);

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
			static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_IDLE.ID);

			m_CurrMonsterAction = &m_IdleAction;
			return;
		}
	} 
}

void MonsterActionMgr::ChangeStateToIdle()
{
	m_BeforeMonsterAction = m_CurrMonsterAction;
	m_CurrMonsterAction = &m_IdleAction;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_IDLE.ID);
}

void MonsterActionMgr::ChangeStateToMove()
{
	m_BeforeMonsterAction = m_CurrMonsterAction;
	m_CurrMonsterAction = &m_MoveAction;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
}

void MonsterActionMgr::ChangeStateToChase()
{
	m_BeforeMonsterAction = m_CurrMonsterAction;
	m_CurrMonsterAction = &m_ChaseAction;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
}

void MonsterActionMgr::ChangeStateToSearch()
{
	m_BeforeMonsterAction = m_CurrMonsterAction;
	m_CurrMonsterAction = &m_SearchAction;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
}

void MonsterActionMgr::ChangeStateToDead()
{
	m_BeforeMonsterAction = m_CurrMonsterAction;
	m_CurrMonsterAction = &m_DeadAction;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_DEAD.ID);
}

void MonsterActionMgr::ChangeStateToHit()
{
	m_BeforeMonsterAction = m_CurrMonsterAction;
	m_CurrMonsterAction = &m_HitAction;
	// static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_HIT.ID);
}

void MonsterActionMgr::ChangeStateToAttack()
{
	m_BeforeMonsterAction = m_CurrMonsterAction;
	m_CurrMonsterAction = &m_AttackAction;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_ATTACK.ID);
}
