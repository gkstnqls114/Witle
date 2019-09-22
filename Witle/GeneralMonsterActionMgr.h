#pragma once
#include "ChaseAction.h"
#include "IdleAction.h"
#include "DeadAction.h"
#include "MoveAction.h"
#include "SearchAction.h"
#include "HitAction.h"
#include "AttackAction.h"

#include "NoneAction.h"

#include "ActionMgr.h"

class Action;
class Movement;
class Player;

// MonsterMovement를 인자로 받아 가속도를 처리해주는 클래스.
class GeneralMonsterActionMgr
	: public ActionMgr
{
	GeneralMonsterNoneAction	       m_GeneralMonsterErrorAction;

	IdleAction      m_IdleAction;
	MoveAction      m_MoveAction;
	ChaseAction	    m_ChaseAction;
	SearchAction    m_SearchAction;
	DeadAction      m_DeadAction;
	HitAction       m_HitAction;
	AttackAction    m_AttackAction;
	 
	float m_TotalTime{ 0.f };

public:
	virtual void UpdateState(float fElpasedTime) override;
	 
	// 이후에 사용할 액션 ID와 현재 사용하는 액션 ID 다른지 알아낸다. 다르다면 true
	virtual bool isDifferAfterAndCurrent() const override;

	// AfterAction을 각 클래스에 맞는 NoneAction으로 설정한다.
	virtual void SetUpAfterAction() override;

	// AfterAction이 각 클래스에 맞는 NoneAction이라면 true를 반환한다.
	virtual bool isAfterNoneAction() override;
;

	virtual void Init() override;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	GeneralMonsterActionMgr(GameObject* pOwner, float idleTime, float moveTime) : 
		ActionMgr(pOwner) ,
		m_GeneralMonsterErrorAction(pOwner),
		m_IdleAction(pOwner, idleTime),
		m_MoveAction(pOwner, moveTime),
		m_ChaseAction(pOwner),
		m_SearchAction(pOwner),
		m_DeadAction(pOwner),
		m_HitAction(pOwner),
		m_AttackAction(pOwner)
	{  
		m_BeforeAction = &m_GeneralMonsterErrorAction;
		m_CurrAction = &m_GeneralMonsterErrorAction;
		m_AfterAction = &m_GeneralMonsterErrorAction;
	};
	virtual ~GeneralMonsterActionMgr() {};
	 

	float GetTotalTime() const { return m_TotalTime; }
	void SetZeroTotalTime() { m_TotalTime = 0.f; }


	// State Check //////////////////////////////////////

	bool isIdle() const { return (m_AfterAction == &m_IdleAction); }
	bool isMove() const { return (m_AfterAction == &m_MoveAction); }
	bool isChase() const { return (m_AfterAction == &m_ChaseAction); }
	bool isSearch() const { return (m_AfterAction == &m_SearchAction); }
	bool isDead() const { return (m_AfterAction == &m_DeadAction); }
	bool isHit() const { return (m_AfterAction == &m_HitAction); }
	bool isAttack() const { return (m_AfterAction == &m_AttackAction); }
	
	// State Check //////////////////////////////////////

	
	// ChageState //////////////////////////////////////
	
	// 이전 상태로 되돌아갑니다.
	void ChangeStateBefore();
	void ChangeStateToIdle();
	void ChangeStateToMove();
	void ChangeStateToChase();
	void ChangeStateToSearch();
	void ChangeStateToDead();
	void ChangeStateToHit();
	void ChangeStateToAttack();

	// ChageState //////////////////////////////////////
};