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
protected:
	MonsterAction*  m_CurrMonsterAction{ nullptr };
	MonsterAction*  m_BeforeMonsterAction{ nullptr };
	 
public:
	virtual void UpdateState(float fElpasedTime) = 0;

protected:
	void ChangeAction(MonsterAction* );

public:
	MonsterActionMgr(GameObject* pOwner) : ComponentBase(pOwner) { };
	virtual ~MonsterActionMgr() {};

	// Update 수행 이전 반드시 호출
	void UpdateVelocity(float fElpasedTime, MonsterMovement* movement);

};