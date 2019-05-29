#pragma once
#include "ChaseAction.h"
#include "IdleAction.h"
#include "DeadAction.h"
#include "MoveAction.h"
#include "SearchAction.h"
#include "HitAction.h"

#include "ComponentBase.h"

class MonsterAction;
class MonsterMovement;
class Player;

// MonsterMovement를 인자로 받아 가속도를 처리해주는 클래스.
class MonsterActionMgr
	: public ComponentBase
{
	MonsterAction*  m_CurrMonsterAction{ nullptr };

	IdleAction      m_IdleAction;
	MoveAction      m_MoveAction;
	ChaseAction	    m_ChaseAction;
	SearchAction    m_SearchAction;
	DeadAction      m_DeadAction;
	HitAction       m_HitAction;
	 
	float m_TotalTime{ 0.f };
	const float m_IdleTime{ 1.f }; //Idle 상태로 있는 시간
	const float m_MoveTime{ 6.f }; //Move 상태로 있는 시간

private:
	bool IsNearPlayer(const Player * target, float distance);

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	MonsterActionMgr(GameObject* pOwner) : 
		ComponentBase(pOwner) ,
		m_IdleAction(pOwner),
		m_MoveAction(pOwner),
		m_ChaseAction(pOwner),
		m_SearchAction(pOwner),
		m_DeadAction(pOwner),
		m_HitAction(pOwner)
	{
		m_CurrMonsterAction = &m_IdleAction;
	};
	virtual ~MonsterActionMgr() {};

	// Update 수행 이전 반드시 호출
	void UpdateVelocity(float fElpasedTime, MonsterMovement* movement);
	void UpdateState(float fElpasedTime);

	// ChageState
	void ChangeStateToIdle();
	void ChangeStateToMove();
	void ChangeStateToChase();
	void ChangeStateToSearch();
	void ChangeStateToDead();
	void ChangeStateToHit();
};