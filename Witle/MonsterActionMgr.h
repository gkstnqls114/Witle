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