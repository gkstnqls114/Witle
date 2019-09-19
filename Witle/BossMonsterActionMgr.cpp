#include "stdafx.h"

#include "Object.h"
#include "PlayerManager.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "MonsterStatus.h"
#include "Monster.h"
#include "BossSkillMgr.h"

#include "BossMonsterActionMgr.h"

#include "SoundManager.h"

void BossMonsterActionMgr::UpdateState(float fElpasedTime)
{
	// 만약 무조건 체력이 0라면 상태를 전환하고 죽는다.
	if (Is_BossDead()) return;

	if (static_cast<Monster*>(m_pOwner)->GetStatus()->GetGuage() <= 0.f)
	{
		ChangeBossStateToDead();
		return;
	}

	m_TotalTime += fElpasedTime;

	static_cast<BossMonsterAction*>(m_CurrAction)->UpdateState(fElpasedTime, this);
}

void BossMonsterActionMgr::ChangeBossStateBefore()
{
	// 상태가 이전하고 다른 경우에만 서로 변경한다.
	if (m_CurrAction != m_BeforeAction)
	{
		Action* temp = m_CurrAction;
		m_CurrAction = m_BeforeAction;
		m_BeforeAction = temp;
	}

	if (m_CurrAction == &m_BossIdleAction)
	{
		ChangeBossStateToIdle(); 
	}
	else if (m_CurrAction == &m_BossMoveAction)
	{
		ChangeBossStateToMove(); 
	}
	else if (m_CurrAction == &m_BossChaseAction)
	{
		ChangeBossStateToChase();
	} 
	 else if (m_CurrAction == &m_BossBuf)
	 {
	 	ChangeBossStateToBuf();
	 }
	 else if (m_CurrAction == &m_BossDead)
	 {
		ChangeBossStateToDead();
	 }

	 else if (m_CurrAction == &m_BossStone)
	{
		ChangeBossStateToStone();
	}
	else if (m_CurrAction == &m_BossSkillBreath)
	{
		ChangeBossStateToBreath();
	}
	else if (m_CurrAction == &m_BossSkilldownstroke)
	{
		ChangeBossStateToDownStroke();
	}
	else if (m_CurrAction == &m_BossSkIllTailAttack)
	{
		ChangeBossStateToTailAttack();
	}
	else if (m_CurrAction == &m_BossSkIlldash)
	{
		ChangeBossStateToRush();
	} 
}
 
// 기본
void BossMonsterActionMgr::ChangeBossStateToIdle()
{ 
	bool isChanged = ChangeAction(&m_BossIdleAction);
	if (!isChanged) return;
	m_CurrAction->Init();
	m_CurrActionID = BOSS_IDLE.ID;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_IDLE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false); 
}

// 이동
void BossMonsterActionMgr::ChangeBossStateToMove()
{ 
	bool isChanged = ChangeAction(&m_BossMoveAction);
	if (!isChanged) return;
	m_CurrAction->Init();
	m_CurrActionID = BOSS_MOVE.ID;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_MOVE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false); 
}

// 추격
void BossMonsterActionMgr::ChangeBossStateToChase()
{ 
	bool isChanged = ChangeAction(&m_BossChaseAction);
	if (!isChanged) return;
	m_CurrAction->Init();
	m_CurrActionID = BOSS_CHASE.ID;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_CHASE.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false); 
}
  
void BossMonsterActionMgr::ChangeBossStateToBreath()
{ 
	bool isChanged = ChangeAction(&m_BossSkillBreath);
	if (!isChanged) return;
	m_CurrAction->Init();
	m_CurrActionID = BOSS_BREATH.ID;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_BREATH.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true); 
}
 
void BossMonsterActionMgr::ChangeBossStateToDownStroke()
{ 
	bool isChanged = ChangeAction(&m_BossSkilldownstroke);
	if (!isChanged) return;
	m_CurrAction->Init();
	m_CurrActionID = BOSS_DOWNSTORK.ID;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_DOWNSTORK.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true); 
}
 
void BossMonsterActionMgr::ChangeBossStateToTailAttack()
{ 
	bool isChanged = ChangeAction(&m_BossSkIllTailAttack);
	if (!isChanged) return;
	m_CurrAction->Init();
	m_CurrActionID = BOSS_TAILATTACK.ID;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_TAILATTACK.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true); 
}
 
void BossMonsterActionMgr::ChangeBossStateToRush()
{ 
	bool isChanged = ChangeAction(&m_BossSkIlldash);
	if (!isChanged) return;
	m_CurrAction->Init();
	m_CurrActionID = BOSS_RUSH.ID;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_RUSH.ID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true);	 
}

void BossMonsterActionMgr::ChangeBossStateToStone()
{ 
	bool isChanged = ChangeAction(&m_BossStone);
	if (!isChanged) return;
	m_CurrAction->Init();
	m_CurrActionID = BOSS_STONE.ID;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_STONE.ID); 
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void BossMonsterActionMgr::ChangeBossStateToDead()
{ 
	bool isChanged = ChangeAction(&m_BossDead);
	if (!isChanged) return;
	m_CurrAction->Init();
	m_CurrActionID = BOSS_DEAD.ID;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_DEAD.ID);
}

void BossMonsterActionMgr::ChangeBossStateToBuf()
{ 
	bool isChanged = ChangeAction(&m_BossBuf);
	if (!isChanged) return;
	m_CurrAction->Init();
	m_CurrActionID = BOSS_BUF.ID;
	static_cast<Monster*>(m_pOwner)->SetAnimationState(BOSS_BUF.ID);
}
