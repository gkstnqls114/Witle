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

// 기본
void BossMonsterActionMgr::BossIdleAction()
{
	ChangeAction(&BossIdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_IDLE.ID);
}

// 이동
void BossMonsterActionMgr::BossMoveAction()
{
	ChangeAction(&BossMoveAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_MOVE.ID);
}

// 추격
void BossMonsterActionMgr::BossChaseAction()
{
	ChangeAction(&BossChaseAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_CHASE.ID);
}

// 죽음
void BossMonsterActionMgr::BossDeadAction()
{
	ChangeAction(&BossDeadAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_DEAD.ID);
}

// 스킬 0
void BossMonsterActionMgr::BossSkillAction0()
{
	ChangeAction(&BossSkillAction0);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL0.ID);
}

// 스킬 1
void BossMonsterActionMgr::BossSkillAction1()
{
	ChangeAction(&BossSkillAction1);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL1.ID);
}

// 스킬 2
void BossMonsterActionMgr::BossSkillAction2()
{
	ChangeAction(&BossSkillAction2);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL2.ID);
}

// 스킬 3
void BossMonsterActionMgr::BossSkillAction3()
{
	ChangeAction(&BossSkillAction3);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL3.ID);
}

// 스킬 4
void BossMonsterActionMgr::BossSkillAction4()
{
	ChangeAction(&BossSkillAction4);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL4.ID);
}

// 스킬 5
void BossMonsterActionMgr::BossSkillAction5()
{
	ChangeAction(&BossSkillAction5);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL5.ID);
}

// 스킬 6
void BossMonsterActionMgr::BossSkillAction6()
{
	ChangeAction(&BossSkillAction6);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL6.ID);
}

// 스킬 7
void BossMonsterActionMgr::BossSkillAction7()
{
	ChangeAction(&BossSkillAction7);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL7.ID);
}

// 스킬 8
void BossMonsterActionMgr::BossSkillAction8()
{
	ChangeAction(&BossSkillAction8);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL8.ID);
}

// 스킬 9
void BossMonsterActionMgr::BossSkillAction9()
{
	ChangeAction(&BossSkillAction9);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL9.ID);
}