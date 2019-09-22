#include "stdafx.h"

#include "Object.h"
#include "PlayerManager.h"
#include "Player.h"
#include "RecognitionRange.h"
#include "MonsterStatus.h"
#include "Monster.h"
#include "BossSkillMgr.h"

#include "Dragon.h"

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

	static_cast<BossMonsterAction*>(m_AfterAction)->UpdateState(fElpasedTime, this);
}
 
bool BossMonsterActionMgr::isDifferAfterAndCurrent() const
{ 
	if ((static_cast<BossMonsterAction*>(m_AfterAction)->m_ActionID == ENUM_BOSSMONSTER_ACTIONID::BOSSMONSTER_ACTIONID_ERROR) && (static_cast<BossMonsterAction*>(m_CurrAction)->m_ActionID == ENUM_BOSSMONSTER_ACTIONID::BOSSMONSTER_ACTIONID_ERROR)) return true;
	if ((static_cast<BossMonsterAction*>(m_AfterAction)->m_ActionID) != (static_cast<BossMonsterAction*>(m_CurrAction)->m_ActionID)) return true;
	else return false;
}

void BossMonsterActionMgr::SetUpAfterAction()
{
	m_AfterAction = &m_BossMonsterErrorAction;
}

bool BossMonsterActionMgr::isAfterNoneAction()
{
	return isSameAfterAction(&m_BossMonsterErrorAction);
}

void BossMonsterActionMgr::SetTrackAnimationSet() const
{
	static_cast<Dragon*>(m_pOwner)->SetAnimationID(m_CurrAction->m_AnimationID);
}

void BossMonsterActionMgr::Init()
{
	m_AfterAction = &m_BossMonsterErrorAction;
	m_CurrAction = &m_BossMonsterErrorAction;
	m_BeforeAction = &m_BossMonsterErrorAction;
	ChangeBossStateToStone();
}

void BossMonsterActionMgr::ChangeBossStateBefore()
{
	// 상태가 이전하고 다른 경우에만 서로 변경한다.
	if (m_AfterAction != m_BeforeAction)
	{
		Action* temp = m_AfterAction;
		m_AfterAction = m_BeforeAction;
		m_BeforeAction = temp;
	}

	if (m_AfterAction == &m_BossIdleAction)
	{
		ChangeBossStateToIdle(); 
	}
	else if (m_AfterAction == &m_BossMoveAction)
	{
		ChangeBossStateToMove(); 
	}
	else if (m_AfterAction == &m_BossChaseAction)
	{
		ChangeBossStateToChase();
	} 
	 else if (m_AfterAction == &m_BossBuf)
	 {
	 	ChangeBossStateToBuf();
	 }
	 else if (m_AfterAction == &m_BossDead)
	 {
		ChangeBossStateToDead();
	 }

	 else if (m_AfterAction == &m_BossStone)
	{
		ChangeBossStateToStone();
	}
	else if (m_AfterAction == &m_BossSkillBreath)
	{
		ChangeBossStateToBreath();
	}
	else if (m_AfterAction == &m_BossSkilldownstroke)
	{
		ChangeBossStateToDownStroke();
	}
	else if (m_AfterAction == &m_BossSkIllTailAttack)
	{
		ChangeBossStateToTailAttack();
	}
	else if (m_AfterAction == &m_BossSkIllRush)
	{
		ChangeBossStateToRush();
	} 
}
 
// 기본
void BossMonsterActionMgr::ChangeBossStateToIdle()
{ 
	bool isChanged = ChangeAfterAction(&m_BossIdleAction);
	if (!isChanged) return;

	m_AfterAction->Init(); 
	static_cast<Monster*>(m_pOwner)->SetAnimationID(m_BossIdleAction.m_AnimationID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false); 
}

// 이동
void BossMonsterActionMgr::ChangeBossStateToMove()
{ 
	bool isChanged = ChangeAfterAction(&m_BossMoveAction);
	if (!isChanged) return;

	m_AfterAction->Init(); 
	static_cast<Monster*>(m_pOwner)->SetAnimationID(m_BossMoveAction.m_AnimationID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false); 
}

// 추격
void BossMonsterActionMgr::ChangeBossStateToChase()
{ 
	bool isChanged = ChangeAfterAction(&m_BossChaseAction);
	if (!isChanged) return;
	m_AfterAction->Init();

	static_cast<Monster*>(m_pOwner)->SetAnimationID(m_BossChaseAction.m_AnimationID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false); 
}
  
void BossMonsterActionMgr::ChangeBossStateToBreath()
{ 
	bool isChanged = ChangeAfterAction(&m_BossSkillBreath);
	if (!isChanged) return;
	m_AfterAction->Init();

	static_cast<Monster*>(m_pOwner)->SetAnimationID(m_BossSkillBreath.m_AnimationID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true); 
}
 
void BossMonsterActionMgr::ChangeBossStateToDownStroke()
{ 
	bool isChanged = ChangeAfterAction(&m_BossSkilldownstroke);
	if (!isChanged) return;
	m_AfterAction->Init();

	static_cast<Monster*>(m_pOwner)->SetAnimationID(m_BossSkilldownstroke.m_AnimationID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true); 
}
 
void BossMonsterActionMgr::ChangeBossStateToTailAttack()
{ 
	bool isChanged = ChangeAfterAction(&m_BossSkIllTailAttack);
	if (!isChanged) return;
	m_AfterAction->Init();

	static_cast<Monster*>(m_pOwner)->SetAnimationID(m_BossSkIllTailAttack.m_AnimationID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true); 
}
 
void BossMonsterActionMgr::ChangeBossStateToRush()
{ 
	bool isChanged = ChangeAfterAction(&m_BossSkIllRush);
	if (!isChanged) return;
	m_AfterAction->Init();

	static_cast<Monster*>(m_pOwner)->SetAnimationID(m_BossSkIllRush.m_AnimationID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(true);	 
}

void BossMonsterActionMgr::ChangeBossStateToStone()
{ 
	bool isChanged = ChangeAfterAction(&m_BossStone);
	if (!isChanged) return;
	m_AfterAction->Init();

	static_cast<Monster*>(m_pOwner)->SetAnimationID(m_BossStone.m_AnimationID);
	static_cast<Monster*>(m_pOwner)->SetisAttacking(false);
}

void BossMonsterActionMgr::ChangeBossStateToDead()
{ 
	bool isChanged = ChangeAfterAction(&m_BossDead);
	if (!isChanged) return;
	m_AfterAction->Init();

	static_cast<Monster*>(m_pOwner)->SetAnimationID(m_BossDead.m_AnimationID);
}

void BossMonsterActionMgr::ChangeBossStateToBuf()
{ 
	bool isChanged = ChangeAfterAction(&m_BossBuf);
	if (!isChanged) return;
	m_AfterAction->Init();

	static_cast<Monster*>(m_pOwner)->SetAnimationID(m_BossBuf.m_AnimationID);
}
