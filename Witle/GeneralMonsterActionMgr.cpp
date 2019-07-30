#include "stdafx.h"

#include "Object.h"
#include "PlayerManager.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "MonsterStatus.h"
#include "Monster.h" 

#include "GeneralMonsterActionMgr.h"

#include "SoundManager.h"

void GeneralMonsterActionMgr::UpdateState(float fElpasedTime)
{
	m_TotalTime += fElpasedTime;

	// Dead ���¸� �����ϰ� ���������� ������
	if (m_CurrMonsterAction != &m_DeadAction && static_cast<Monster*>(m_pOwner)->GetStatus()->m_Guage <= 0)
	{
		ChangeStateToDead();
	}

	static_cast<GeneralMonsterAction*>(m_CurrMonsterAction)->UpdateState(fElpasedTime, this);

	// CHASE���� IDLE��...
	//if (m_CurrMonsterAction == &m_ChaseAction
	//	&& !IsNearPlayer(PlayerManager::GetMainPlayer(), pMonsterOwner->GetRecognitionRange()->m_RecognitionRange))
	//{
	//	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_IDLE.ID); 
	//	m_TotalTime = 0.f;
	//	m_CurrMonsterAction = &m_IdleAction;
	//	return;
	//} 
}

void GeneralMonsterActionMgr::ChangeStateBefore()
{
	// ���°� �����ϰ� �ٸ� ��쿡�� ���� �����Ѵ�.
	if (m_CurrMonsterAction != m_BeforeMonsterAction)
	{
		MonsterAction* temp = m_CurrMonsterAction;
		m_CurrMonsterAction = m_BeforeMonsterAction;
		m_BeforeMonsterAction = temp;
	}

	if (m_CurrMonsterAction == &m_IdleAction)
	{
		ChangeStateToIdle();
	}
	else if (m_CurrMonsterAction == &m_MoveAction)
	{
		ChangeStateToMove();
	}
	else if (m_CurrMonsterAction == &m_ChaseAction)
	{
		ChangeStateToChase();
	}
	else if (m_CurrMonsterAction == &m_SearchAction)
	{
		ChangeStateToSearch();
	}
	else if (m_CurrMonsterAction == &m_DeadAction)
	{
		ChangeStateToDead();
	}
	else if (m_CurrMonsterAction == &m_HitAction)
	{
		ChangeStateToHit();
	}
	else if (m_CurrMonsterAction == &m_AttackAction)
	{
		ChangeStateToAttack();
	}
}

void GeneralMonsterActionMgr::ChangeStateToIdle()
{
	ChangeAction(&m_IdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_IDLE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
	// printf("���� ���", "%d");
}

void GeneralMonsterActionMgr::ChangeStateToMove()
{
	ChangeAction(&m_MoveAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
	// printf("���� �̵�", "%d");
}

void GeneralMonsterActionMgr::ChangeStateToChase()
{
	ChangeAction(&m_ChaseAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void GeneralMonsterActionMgr::ChangeStateToSearch()
{
	ChangeAction(&m_SearchAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void GeneralMonsterActionMgr::ChangeStateToDead()
{
	ChangeAction(&m_DeadAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_DEAD.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
	//SoundManager::GetInstance()->Stop(ENUM_SOUND::SPACE_MOVE);
}

void GeneralMonsterActionMgr::ChangeStateToHit()
{
	ChangeAction(&m_HitAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_HIT.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void GeneralMonsterActionMgr::ChangeStateToAttack()
{
	ChangeAction(&m_AttackAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_ATTACK.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true);
	printf("���� ����\n");
}
