#pragma once
#include "ChaseAction.h"
#include "IdleAction.h"
#include "DeadAction.h"
#include "MoveAction.h"
#include "SearchAction.h"
#include "HitAction.h"
#include "AttackAction.h"

#include "ComponentBase.h"

class MonsterAction;
class MonsterMovement;
class Player;

// MonsterMovement를 인자로 받아 가속도를 처리해주는 클래스.
class MonsterActionMgr
	: public ComponentBase
{
	MonsterAction*  m_CurrMonsterAction{ nullptr };
	MonsterAction*  m_BeforeMonsterAction{ nullptr };

	IdleAction      m_IdleAction;
	MoveAction      m_MoveAction;
	ChaseAction	    m_ChaseAction;
	SearchAction    m_SearchAction;
	DeadAction      m_DeadAction;
	HitAction       m_HitAction;
	AttackAction    m_AttackAction;
	 
	float m_TotalTime{ 0.f };
	 
public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};
	 
private:
	void ChangeAction(MonsterAction* );

public:
	MonsterActionMgr(GameObject* pOwner) : 
		ComponentBase(pOwner) ,
		m_IdleAction(pOwner),
		m_MoveAction(pOwner),
		m_ChaseAction(pOwner),
		m_SearchAction(pOwner),
		m_DeadAction(pOwner),
		m_HitAction(pOwner),
		m_AttackAction(pOwner)
	{
		m_CurrMonsterAction = &m_IdleAction;
	};
	virtual ~MonsterActionMgr() {};

	// Update 수행 이전 반드시 호출
	void UpdateVelocity(float fElpasedTime, MonsterMovement* movement);
	void UpdateState(float fElpasedTime);
	
	float GetTotalTime() const { return m_TotalTime; }
	void SetZeroTotalTime() { m_TotalTime = 0.f; }

	// State Check //////////////////////////////////////

	bool isIdle() const { return (m_CurrMonsterAction == &m_IdleAction); }
	bool isMove() const { return (m_CurrMonsterAction == &m_MoveAction); }
	bool isChase() const { return (m_CurrMonsterAction == &m_ChaseAction); }
	bool isSearch() const { return (m_CurrMonsterAction == &m_SearchAction); }
	bool isDead() const { return (m_CurrMonsterAction == &m_DeadAction); }
	bool isHit() const { return (m_CurrMonsterAction == &m_HitAction); }
	bool isAttack() const { return (m_CurrMonsterAction == &m_AttackAction); }
	
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