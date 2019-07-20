#include "stdafx.h"

#include "Object.h"
#include "PlayerManager.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "MonsterStatus.h"
#include "Monster.h"

#include "BossMonsterActionMgr.h"

void BossMonsterActionMgr::UpdateState(float fElpasedTime)
{
	static_cast<BossMonsterAction*>(m_CurrMonsterAction)->UpdateState(fElpasedTime, this);
}


void BossMonsterActionMgr::ChangeStateToSample_1()
{
	ChangeAction(&m_BossSampleAction_1);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_IDLE.ID);
}

void BossMonsterActionMgr::ChangeStateToSample_2()
{
	ChangeAction(&m_BossSampleAction_2);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(SPACECAT_MOVE.ID);
}

// �⺻
void BossMonsterActionMgr::BossIdleAction()
{
	ChangeAction(&m_BossIdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_IDLE.ID);
}

// �̵�
void BossMonsterActionMgr::BossMoveAction()
{
	ChangeAction(&m_BossMoveAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_MOVE.ID);
}

// �߰�
void BossMonsterActionMgr::BossChaseAction()
{
	ChangeAction(&m_BossIdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_CHASE.ID);
}

// ����
void BossMonsterActionMgr::BossDeadAction()
{
	ChangeAction(&m_BossIdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_DEAD.ID);
}

// ��ų 0
void BossMonsterActionMgr::BossSkillAction0()
{
	ChangeAction(&m_BossIdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL0.ID);
}

// ��ų 1
void BossMonsterActionMgr::BossSkillAction1()
{
	ChangeAction(&m_BossIdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL1.ID);
}

// ��ų 2
void BossMonsterActionMgr::BossSkillAction2()
{
	ChangeAction(&m_BossIdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL2.ID);
}

// ��ų 3
void BossMonsterActionMgr::BossSkillAction3()
{
	ChangeAction(&m_BossIdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL3.ID);
}

// ��ų 4
void BossMonsterActionMgr::BossSkillAction4()
{
	ChangeAction(&m_BossIdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL4.ID);
}

// ��ų 5
void BossMonsterActionMgr::BossSkillAction5()
{
	ChangeAction(&m_BossIdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL5.ID);
}

// ��ų 6
void BossMonsterActionMgr::BossSkillAction6()
{
	ChangeAction(&m_BossIdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL6.ID);
}

// ��ų 7
void BossMonsterActionMgr::BossSkillAction7()
{
	ChangeAction(&m_BossIdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL7.ID);
}

// ��ų 8
void BossMonsterActionMgr::BossSkillAction8()
{
	ChangeAction(&m_BossIdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL8.ID);
}

// ��ų 9
void BossMonsterActionMgr::BossSkillAction9()
{
	ChangeAction(&m_BossIdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL9.ID);
}