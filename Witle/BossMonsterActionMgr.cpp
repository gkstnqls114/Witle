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
	}
	else if (m_CurrMonsterAction == &m_BossMoveAction)
	{
		ChangeBossStateToMove(); 
	}
	else if (m_CurrMonsterAction == &m_BossChaseAction)
	{
		ChangeBossStateToChase();
	} 
	 else if (m_CurrMonsterAction == &m_BossBuf)
	 {
	 	ChangeBossStateToBuf();
	 }
	 else if (m_CurrMonsterAction == &m_BossDead)
	 {
		ChangeBossStateToDead();
	 }

	 else if (m_CurrMonsterAction == &m_BossStone)
	{
		ChangeBossStateToStone();
	}
	else if (m_CurrMonsterAction == &m_BossSkillBreath)
	{
		ChangeBossStateToBreath();
	}
	else if (m_CurrMonsterAction == &m_BossSkilldownstroke)
	{
		ChangeBossStateToDownStroke();
	}
	else if (m_CurrMonsterAction == &m_BossSkIllTailAttack)
	{
		ChangeBossStateToTailAttack();
	}
	else if (m_CurrMonsterAction == &m_BossSkIlldash)
	{
		ChangeBossStateToRush();
	} 
}
 
// 기본
void BossMonsterActionMgr::ChangeBossStateToIdle()
{
	ChangeAction(&m_BossIdleAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_IDLE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false); 
}

// 이동
void BossMonsterActionMgr::ChangeBossStateToMove()
{
	ChangeAction(&m_BossMoveAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_MOVE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false); 
}

// 추격
void BossMonsterActionMgr::ChangeBossStateToChase()
{
	ChangeAction(&m_BossChaseAction);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_CHASE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false); 
}
  
void BossMonsterActionMgr::ChangeBossStateToBreath()
{
	ChangeAction(&m_BossSkillBreath);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_BRETH.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true); 
}
 
void BossMonsterActionMgr::ChangeBossStateToDownStroke()
{
	ChangeAction(&m_BossSkilldownstroke);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_DOWNSTORK.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true); 
}
 
void BossMonsterActionMgr::ChangeBossStateToTailAttack()
{
	ChangeAction(&m_BossSkIllTailAttack);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_TAILATTACK.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true); 
}
 
void BossMonsterActionMgr::ChangeBossStateToRush()
{
	ChangeAction(&m_BossSkIlldash);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_RUSH.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true);
	 
}
void BossMonsterActionMgr::ChangeBossStateToStone()
{
	ChangeAction(&m_BossStone);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_STONE.ID); 
}

void BossMonsterActionMgr::ChangeBossStateToDead()
{
	ChangeAction(&m_BossDead);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_DEAD.ID);
}

void BossMonsterActionMgr::ChangeBossStateToBuf()
{
	ChangeAction(&m_BossBuf);
	m_CurrMonsterAction->Init();
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_BUF.ID);
}
