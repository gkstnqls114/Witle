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
	// ���°� �����ϰ� �ٸ� ��쿡�� ���� �����Ѵ�.
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
	else if (m_CurrMonsterAction == &m_BossSkillBreath)
	{
		ChangeBossStateToAction0();
	}
	else if (m_CurrMonsterAction == &m_BossSkilldownstroke)
	{
		ChangeBossStateToAction1();
	}
	else if (m_CurrMonsterAction == &m_BossSkIllTailAttack)
	{
		ChangeBossStateToAction2();
	}
	else if (m_CurrMonsterAction == &m_BossSkIlldash)
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

// �⺻
void BossMonsterActionMgr::ChangeBossStateToIdle()
{
	ChangeAction(&m_BossIdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_IDLE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
	printf("���� ���\n");
}

// �̵�
void BossMonsterActionMgr::ChangeBossStateToMove()
{
	ChangeAction(&m_BossMoveAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_MOVE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
	printf("���� �̵�\n");
}

// �߰�
void BossMonsterActionMgr::ChangeBossStateToChase()
{
	ChangeAction(&m_BossChaseAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_CHASE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
	printf("���� ����\n");
}

// // ����
// void BossMonsterActionMgr::ChangeBossStateToDead()
// {
// 	ChangeAction(&m_BossIdleAction);
// 	m_CurrMonsterAction->Init();
// 	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_DEAD.ID);
// }

// ��ų 0
void BossMonsterActionMgr::ChangeBossStateToAction0()
{
	ChangeAction(&m_BossSkillBreath);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL0.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true);
	printf("���� �극��\n");
}

// ��ų 1
void BossMonsterActionMgr::ChangeBossStateToAction1()
{
	ChangeAction(&m_BossSkilldownstroke);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL1.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true);
	printf("���� �������\n");
}

// ��ų 2
void BossMonsterActionMgr::ChangeBossStateToAction2()
{
	ChangeAction(&m_BossSkIllTailAttack);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL2.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true);
	printf("���� ����ȸ��\n");
}

// ��ų 3
void BossMonsterActionMgr::ChangeBossStateToAction3()
{
	ChangeAction(&m_BossSkIlldash);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL3.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true);

	printf("���� ����\n");
}

// ��ų 4
void BossMonsterActionMgr::ChangeBossStateToAction4()
{
	ChangeAction(&m_BossSkillAction4);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL4.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true);
	printf("���� ��ų 4\n");
}

// // ��ų 5
// void BossMonsterActionMgr::ChangeBossStateToAction5()
// {
// 	ChangeAction(&m_BossSkillAction5);
// 	m_CurrMonsterAction->Init();
// 	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL5.ID);
// }
// 
// // ��ų 6
// void BossMonsterActionMgr::ChangeBossStateToAction6()
// {
// 	ChangeAction(&m_BossSkillAction6);
// 	m_CurrMonsterAction->Init();
// 	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL6.ID);
// }
// 
// // ��ų 7
// void BossMonsterActionMgr::ChangeBossStateToAction7()
// {
// 	ChangeAction(&m_BossSkillAction7);
// 	m_CurrMonsterAction->Init();
// 	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL7.ID);
// }
// 
// // ��ų 8
// void BossMonsterActionMgr::ChangeBossStateToAction8()
// {
// 	ChangeAction(&m_BossSkillAction8);
// 	m_CurrMonsterAction->Init();
// 	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL8.ID);
// }
// 
// // ��ų 9
// void BossMonsterActionMgr::ChangeBossStateToAction9()
// {
// 	ChangeAction(&m_BossSkillAction9);
// 	m_CurrMonsterAction->Init();
// 	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_SKILL9.ID);
// }