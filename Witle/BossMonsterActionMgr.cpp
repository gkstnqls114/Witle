#include "stdafx.h"

#include "Object.h"
#include "PlayerManager.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "MonsterStatus.h"
#include "Monster.h"

#include "BossMonsterActionMgr.h"

#include "SoundManager.h"

void BossMonsterActionMgr::UpdateState(float fElpasedTime)
{
	m_TotalTime += fElpasedTime;

	static_cast<BossMonsterAction*>(m_CurrMonsterAction)->UpdateState(fElpasedTime, this);
}

void BossMonsterActionMgr::ChangeBossStateBefore()
{
	// 상태가 이전하고 다른 경우에만 서로 변경한다.
	if (m_CurrMonsterAction != m_BeforeMonsterAction)
	{
		MonsterAction* temp = m_CurrMonsterAction;
		m_CurrMonsterAction = m_BeforeMonsterAction;
		m_BeforeMonsterAction = temp;
	}

	if (m_CurrMonsterAction == &m_BossIdleAction)
	{
		ChangeBossStateToIdle();
		//SoundManager::GetInstance()->Stop(ENUM_SOUND::MUSHROOM_MOVE);
	}
	else if (m_CurrMonsterAction == &m_BossMoveAction)
	{
		ChangeBossStateToMove();
		//SoundManager::GetInstance()->Play(ENUM_SOUND::MUSHROOM_MOVE);
	}
	else if (m_CurrMonsterAction == &m_BossChaseAction)
	{
		ChangeBossStateToChase();
	}
	// else if (m_CurrMonsterAction == &m_BossSearchAction)
	// {
	// 	ChangeBossStateToSearch();
	// }
	// else if (m_CurrMonsterAction == &m_BossDeadAction)
	// {
	// 	ChangeBossStateToDead();
	// }
	// else if (m_CurrMonsterAction == &m_BossHitAction)
	// {
	// 	ChangeBossStateToHit();
	// }
	else if (m_CurrMonsterAction == &m_BossSkillAction0)
	{
		ChangeBossStateToAction0();
	}
	else if (m_CurrMonsterAction == &m_BossSkillAction1)
	{
		ChangeBossStateToAction1();
	}
	else if (m_CurrMonsterAction == &m_BossSkillAction2)
	{
		ChangeBossStateToAction2();
	}
	else if (m_CurrMonsterAction == &m_BossSkillAction3)
	{
		ChangeBossStateToAction3();
	}
	else if (m_CurrMonsterAction == &m_BossSkillAction4)
	{
		ChangeBossStateToAction4();
	}
}

void BossMonsterActionMgr::ChangeStateToSample_1()
{
	ChangeAction(&m_BossSampleAction_1);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_IDLE.ID);
}

void BossMonsterActionMgr::ChangeStateToSample_2()
{
	ChangeAction(&m_BossSampleAction_2);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_MOVE.ID);
}

// 기본
void BossMonsterActionMgr::ChangeBossStateToIdle()
{
	ChangeAction(&m_BossIdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_IDLE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
	printf("보스 대기\n");
}

// 이동
void BossMonsterActionMgr::ChangeBossStateToMove()
{
	ChangeAction(&m_BossMoveAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_MOVE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
	printf("보스 이동\n");
}

// 추격
void BossMonsterActionMgr::ChangeBossStateToChase()
{
	ChangeAction(&m_BossChaseAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_CHASE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
	printf("보스 추적\n");
}

// // 죽음
// void BossMonsterActionMgr::ChangeBossStateToDead()
// {
// 	ChangeAction(&m_BossIdleAction);
// 	m_CurrMonsterAction->Init();
// 	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_DEAD.ID);
// }

// 스킬 0
void BossMonsterActionMgr::ChangeBossStateToAction0()
{
	ChangeAction(&m_BossSkillAction0);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL0.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true);
	printf("보스 스킬 0\n");
}

// 스킬 1
void BossMonsterActionMgr::ChangeBossStateToAction1()
{
	ChangeAction(&m_BossSkillAction1);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL1.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true);
	printf("보스 스킬 1\n");
}

// 스킬 2
void BossMonsterActionMgr::ChangeBossStateToAction2()
{
	ChangeAction(&m_BossSkillAction2);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL2.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true);
	printf("보스 스킬 2\n");
}

// 스킬 3
void BossMonsterActionMgr::ChangeBossStateToAction3()
{
	ChangeAction(&m_BossSkillAction3);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL3.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true);
	printf("보스 스킬 3\n");
}

// 스킬 4
void BossMonsterActionMgr::ChangeBossStateToAction4()
{
	ChangeAction(&m_BossSkillAction4);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL4.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true);
	printf("보스 스킬 4\n");
}

// // 스킬 5
// void BossMonsterActionMgr::ChangeBossStateToAction5()
// {
// 	ChangeAction(&m_BossSkillAction5);
// 	m_CurrMonsterAction->Init();
// 	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL5.ID);
// }
// 
// // 스킬 6
// void BossMonsterActionMgr::ChangeBossStateToAction6()
// {
// 	ChangeAction(&m_BossSkillAction6);
// 	m_CurrMonsterAction->Init();
// 	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL6.ID);
// }
// 
// // 스킬 7
// void BossMonsterActionMgr::ChangeBossStateToAction7()
// {
// 	ChangeAction(&m_BossSkillAction7);
// 	m_CurrMonsterAction->Init();
// 	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL7.ID);
// }
// 
// // 스킬 8
// void BossMonsterActionMgr::ChangeBossStateToAction8()
// {
// 	ChangeAction(&m_BossSkillAction8);
// 	m_CurrMonsterAction->Init();
// 	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL8.ID);
// }
// 
// // 스킬 9
// void BossMonsterActionMgr::ChangeBossStateToAction9()
// {
// 	ChangeAction(&m_BossSkillAction9);
// 	m_CurrMonsterAction->Init();
// 	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL9.ID);
// }