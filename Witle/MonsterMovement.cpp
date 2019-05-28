#include "stdafx.h"

// Action ////////////////////////
#include "IdleAction.h"
#include "MoveAction.h"
#include "ChaseAction.h"
#include "SearchAction.h"
#include "PlayerManager.h"
#include "DeadAction.h"
// Action ////////////////////////

#include "Monster.h"
#include "RecognitionRange.h"
#include "Player.h"
#include "MonsterMovement.h"

bool MonsterMovement::IsNearPlayer(const Player * target, float distance)
{
	return Vector3::Length(Vector3::Subtract(m_pOwner->GetTransform().GetPosition(), target->GetTransform().GetPosition())) < distance;
}

void MonsterMovement::UpdateVelocity(float fTimeElapsed)
{ 
	m_CurrMonsterAction->UpdateVelocity(fTimeElapsed, this); 
}

void MonsterMovement::ReleaseObjects()
{ 
	if (m_IdleAction)        delete m_IdleAction;
	if (m_MoveAction)        delete m_MoveAction;
	if (m_ChaseAction)       delete m_ChaseAction;
	if (m_SearchAction)      delete m_SearchAction;
	if (m_DeadAction)        delete m_DeadAction;
}

void MonsterMovement::MoveVelocity(const XMFLOAT3 & xmf3Shift)
{
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
}

MonsterMovement::MonsterMovement(GameObject * pOwner)
	:Movement(pOwner)
{
	m_IdleAction = new IdleAction(pOwner);
	m_MoveAction = new MoveAction(pOwner);
	m_ChaseAction = new ChaseAction(pOwner);
	m_SearchAction = new SearchAction(pOwner);
	m_DeadAction = new DeadAction(pOwner);

	m_CurrMonsterAction = m_IdleAction;
}

MonsterMovement::~MonsterMovement()
{

}

void MonsterMovement::Update(float fTimeElapsed)
{
	UpdateVelocity(fTimeElapsed); // State ���¿� ���� Velocity�� ����(Set)�Ѵ�.

	Movement::Update( fTimeElapsed); 
}

void MonsterMovement::UpdateState(float fElpasedTime)
{   
	Monster* pMonsterOwner = static_cast<Monster*>(m_pOwner);

	// � ��Ȳ�̴�����(chase�� �ƴѰ��) �ν� ��ó�� �÷��̾ �ִ� ��� ���󰣴�.
	if (m_CurrMonsterAction != m_ChaseAction &&
		IsNearPlayer(PlayerManager::GetMainPlayer(), pMonsterOwner->GetRecognitionRange()->m_RecognitionRange))
	{
		m_TotalTime = 0.f;
		 
		pMonsterOwner->GetRecognitionRange()->m_TotalTime += fElpasedTime;

		// ���� �νĽð��� �Ǿ��� ���...
		if (pMonsterOwner->GetRecognitionRange()->m_TotalTime >= pMonsterOwner->GetRecognitionRange()->m_RecognitionTime) 
		{
			pMonsterOwner->GetRecognitionRange()->m_TotalTime = 0;
			m_CurrMonsterAction = m_ChaseAction;
		}
		return;
	}
	else if ((m_CurrMonsterAction != m_ChaseAction &&
		!IsNearPlayer(PlayerManager::GetMainPlayer(), pMonsterOwner->GetRecognitionRange()->m_RecognitionRange)))
	{ 
		pMonsterOwner->GetRecognitionRange()->m_TotalTime = 0;
	}
	
	if (m_CurrMonsterAction == m_ChaseAction
		&& !IsNearPlayer(PlayerManager::GetMainPlayer(), pMonsterOwner->GetRecognitionRange()->m_RecognitionRange))
	{
		m_TotalTime = 0.f;
		m_CurrMonsterAction = m_IdleAction;
		return;
	}

	if (m_CurrMonsterAction == m_ChaseAction) return;

	m_TotalTime += fElpasedTime;
	if (m_CurrMonsterAction == m_IdleAction)
	{
		if (m_TotalTime > m_IdleTime)
		{
			m_TotalTime = 0.f;
			m_CurrMonsterAction = m_MoveAction;
			
			MoveAction* pMoveAction = static_cast<MoveAction*>(m_CurrMonsterAction);
			// ���� ������ġ���� �����Ÿ� �־����� ���...
			if (Vector3::Length(m_pOwner->GetTransform().GetPosition(), pMonsterOwner->GetSpawnPoint()) > pMonsterOwner->GetSpawnRange()) 
			{
				// ������ ���� ������ �ݴ�� �̵�
				pMoveAction->SetDirection(
					Vector3::ScalarProduct(pMoveAction->GetDirection(), -1, false)
				);
			}
			else
			{
				// �����Ÿ����� �����Ÿ� �־������� �ƴ϶�� �׳� �������� ���ư���..
				static_cast<MoveAction*>(m_CurrMonsterAction)->SetDirection(XMFLOAT3(50 + rand() % 100, 0, 50 + rand() % 100));
			}
			return;
		}
	}

	if (m_CurrMonsterAction == m_MoveAction)
	{ 
		if (m_TotalTime > m_MoveTime)
		{
			m_TotalTime = 0.f;
			m_CurrMonsterAction = m_IdleAction;
			return;
		}
	}

} 

void MonsterMovement::ReduceVelocity(float fTimeElapsed)
{
	if (Vector3::Length(m_xmf3Velocity) <= 0.f)
	{
		m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
		return; // �������� �ʴ� ������ ��� �׳� �Ѿ��.
	}
	  
	MoveVelocity(Vector3::ScalarProduct(m_xmf3Velocity, -0.2f, false));
	
}
 