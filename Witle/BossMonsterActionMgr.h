#pragma once  
#include "BossIdleAction.h"
#include "BossMoveAction.h"
#include "BossChaseAction.h"
#include"BossSkillBreath.h"
#include"BossSkillDownStroke.h"
#include"BossSkillTailAttack.h"
#include"BossSkillRush.h" 
#include "BossStone.h"
#include "BossDead.h"
#include "BossBuf.h"

#include "NoneAction.h"

#include "ActionMgr.h"

class Action;
class MonsterMovement;
class Player;

// 보스 몬스터용 액션 매니저입니다.
// ActionMgr를 상속받아 현재 액션상태를 관리합니다. 
class BossMonsterActionMgr
	: public ActionMgr
{
	// 현재 사용하는 액션(상태)들 
	BossMonsterNoneAction m_BossMonsterErrorAction;

	BossIdleAction m_BossIdleAction;
	BossMoveAction m_BossMoveAction;
	BossChaseAction m_BossChaseAction;
	BossSkillBreath m_BossSkillBreath;
	BossSkillDownStroke m_BossSkilldownstroke;
	BossSkillTailAttack m_BossSkIllTailAttack;
	BossSkillRush m_BossSkIllRush;
	BossBuf m_BossBuf;
	BossDead m_BossDead;
	BossStone m_BossStone;
	// 현재 사용하는 액션(상태)들

	float m_TotalTime{ 0.f };

public:
	virtual void UpdateState(float fElpasedTime) override;
	 
	// 이후에 사용할 액션 ID와 현재 사용하는 액션 ID 다른지 알아낸다. 다르다면 true
	virtual bool isDifferAfterAndCurrent() const override;

	// AfterAction을 각 클래스에 맞는 NoneAction으로 설정한다.
	virtual void SetUpAfterAction() override;

	// AfterAction이 각 클래스에 맞는 NoneAction이라면 true를 반환한다.
	virtual bool isAfterNoneAction() override;

	virtual void Init() override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	BossMonsterActionMgr(GameObject* pOwner, float IdleTime, float MoveTime) :
		ActionMgr(pOwner), 
		m_BossMonsterErrorAction(pOwner),
		m_BossIdleAction(pOwner, IdleTime),
		m_BossMoveAction(pOwner, MoveTime),
		m_BossChaseAction(pOwner),
		m_BossSkillBreath(pOwner),
		m_BossSkilldownstroke(pOwner),
		m_BossSkIllTailAttack(pOwner),
		m_BossSkIllRush(pOwner),
		m_BossBuf(pOwner),
		m_BossDead(pOwner),
		m_BossStone(pOwner)
	{ 
		m_BeforeAction = &m_BossMonsterErrorAction;
		m_AfterAction = &m_BossMonsterErrorAction;
		m_CurrAction = &m_BossMonsterErrorAction;
	};
	virtual ~BossMonsterActionMgr() {};

	float GetTotalTime() const { return m_TotalTime; }
	void SetZeroTotalTime() { m_TotalTime = 0.f; }
	 
	bool Is_BossIdleAction() const { return (m_AfterAction == &m_BossIdleAction); } // 기본
	bool Is_BossMoveAction() const { return (m_AfterAction == &m_BossMoveAction); } // 움직임
	bool Is_BossChaseAction() const { return (m_AfterAction == &m_BossChaseAction); } // 추격
	bool Is_BossSkillBreath() const { return (m_AfterAction == &m_BossSkillBreath); }// 브레스
	bool Is_BossSkillDownStroke() const { return (m_AfterAction == &m_BossSkilldownstroke); }// 내려찍기
	bool Is_BossSkillTailAttack() const { return (m_AfterAction == &m_BossSkIllTailAttack); }// 꼬리회전
	bool Is_BossSkillRush() const { return (m_AfterAction == &m_BossSkIllRush); }// 돌진
	bool Is_BossDead() const { return (m_AfterAction == &m_BossDead); }// 돌진
	
	void ChangeBossStateBefore();
	void ChangeBossStateToIdle();
	void ChangeBossStateToMove();
	void ChangeBossStateToChase(); 
	void ChangeBossStateToBreath();
	void ChangeBossStateToDownStroke();
	void ChangeBossStateToTailAttack();
	void ChangeBossStateToRush();
	void ChangeBossStateToStone();
	void ChangeBossStateToDead();
	void ChangeBossStateToBuf();
};
