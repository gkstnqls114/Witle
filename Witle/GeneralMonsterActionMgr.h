#pragma once
#include "ChaseAction.h"
#include "IdleAction.h"
#include "DeadAction.h"
#include "MoveAction.h"
#include "SearchAction.h"
#include "HitAction.h"
#include "AttackAction.h"

#include "ErrorAction.h"

#include "ActionMgr.h"

class Action;
class Movement;
class Player;

// MonsterMovement�� ���ڷ� �޾� ���ӵ��� ó�����ִ� Ŭ����.
class GeneralMonsterActionMgr
	: public ActionMgr
{
	GeneralMonsterErrorAction	       m_GeneralMonsterErrorAction;

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

	// ������ ������ Action ID ���°� �޶������� �˾Ƴ���. ���� �ٸ��ٸ� true
	virtual bool isDifferAction() const override;

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
		Init();
	};
	virtual ~GeneralMonsterActionMgr() {};
	 

	float GetTotalTime() const { return m_TotalTime; }
	void SetZeroTotalTime() { m_TotalTime = 0.f; }


	// State Check //////////////////////////////////////

	bool isIdle() const { return (m_CurrAction == &m_IdleAction); }
	bool isMove() const { return (m_CurrAction == &m_MoveAction); }
	bool isChase() const { return (m_CurrAction == &m_ChaseAction); }
	bool isSearch() const { return (m_CurrAction == &m_SearchAction); }
	bool isDead() const { return (m_CurrAction == &m_DeadAction); }
	bool isHit() const { return (m_CurrAction == &m_HitAction); }
	bool isAttack() const { return (m_CurrAction == &m_AttackAction); }
	
	// State Check //////////////////////////////////////

	
	// ChageState //////////////////////////////////////
	
	// ���� ���·� �ǵ��ư��ϴ�.
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