#pragma once  
#include "BossIdleAction.h"
#include "BossMoveAction.h"
#include "BossChaseAction.h"
#include"BossSkillBreath.h"
#include"BossSkillDownStroke.h"
#include"BossSkillTailAttack.h"
#include"BossSkillRush.h"
#include"BossSkillAction4.h"
#include "MonsterActionMgr.h"

class MonsterAction;
class MonsterMovement;
class Player;

// 보스 몬스터용 액션 매니저입니다.
// MonsterActionMgr를 상속받아 현재 액션상태를 관리합니다. 
class BossMonsterActionMgr
	: public MonsterActionMgr
{
	// 현재 사용하는 액션(상태)들 
	BossIdleAction m_BossIdleAction;
	BossMoveAction m_BossMoveAction;
	BossChaseAction m_BossChaseAction;
	BossSkillBreath m_BossSkillBreath;
	BossSkillDownStroke m_BossSkilldownstroke;
	BossSkillTailAttack m_BossSkIllTailAttack;
	BossSkillRush m_BossSkIlldash;
	BossSkillAction4 m_BossSkillAction4;
	// 현재 사용하는 액션(상태)들

	float m_TotalTime{ 0.f };

public:
	virtual void UpdateState(float fElpasedTime) override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	BossMonsterActionMgr(GameObject* pOwner, float IdleTime, float MoveTime) :
		MonsterActionMgr(pOwner), 
		m_BossIdleAction(pOwner, IdleTime),
		m_BossMoveAction(pOwner, MoveTime),
		m_BossChaseAction(pOwner),
		m_BossSkillBreath(pOwner),
		m_BossSkilldownstroke(pOwner),
		m_BossSkIllTailAttack(pOwner),
		m_BossSkIlldash(pOwner),
		m_BossSkillAction4(pOwner)
	{
		m_CurrMonsterAction = &m_BossIdleAction;
	};
	virtual ~BossMonsterActionMgr() {};

	float GetTotalTime() const { return m_TotalTime; }
	void SetZeroTotalTime() { m_TotalTime = 0.f; }
	 
	bool Is_BossIdleAction() const { return (m_CurrMonsterAction == &m_BossIdleAction); } // 기본
	bool Is_BossMoveAction() const { return (m_CurrMonsterAction == &m_BossMoveAction); } // 움직임
	bool Is_BossChaseAction() const { return (m_CurrMonsterAction == &m_BossChaseAction); } // 추격
	// bool Is_BossSearchAction() const { return (m_CurrMonsterAction == &m_BossSearchAction); } // 필요할까?
	// bool Is_BossDeadAction() const { return (m_CurrMonsterAction == &m_BossDeadAction); } // 죽음
	bool Is_BossSkillActionBreath() const { return (m_CurrMonsterAction == &m_BossSkillBreath); }// 브레스
	bool Is_BossSkillAction1() const { return (m_CurrMonsterAction == &m_BossSkilldownstroke); }// 내려찍기
	bool Is_BossSkillActionTailAttack() const { return (m_CurrMonsterAction == &m_BossSkIllTailAttack); }// 꼬리회전
	bool Is_BossSkillActiondash() const { return (m_CurrMonsterAction == &m_BossSkIlldash); }// 돌진
	bool Is_BossSkillAction4() const { return (m_CurrMonsterAction == &m_BossSkillAction4); }// 임시
	// bool Is_BossSkillAction5() const { return (m_CurrMonsterAction == &m_BossSkillAction5); }// 임시
	// bool Is_BossSkillAction6() const { return (m_CurrMonsterAction == &m_BossSkillAction6); }// 임시
	// bool Is_BossSkillAction7() const { return (m_CurrMonsterAction == &m_BossSkillAction7); }// 임시
	// bool Is_BossSkillAction8() const { return (m_CurrMonsterAction == &m_BossSkillAction8); }// 임시
	// bool Is_BossSkillAction9() const { return (m_CurrMonsterAction == &m_BossSkillAction9); }// 임시

	void ChangeBossStateBefore();
	void ChangeBossStateToIdle();
	void ChangeBossStateToMove();
	void ChangeBossStateToChase();
	void ChangeBossStateToSearch();
	void ChangeBossStateToDead();
	void ChangeBossStateToHit();
	void ChangeBossStateToBreth();
	void ChangeBossStateToDownStroke();
	void ChangeBossStateToAction2();
	void ChangeBossStateToAction3();
	void ChangeBossStateToAction4();
};
