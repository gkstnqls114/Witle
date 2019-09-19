#pragma once
#include "ChaseAction.h"
#include "IdleAction.h"
#include "DeadAction.h"
#include "MoveAction.h"
#include "SearchAction.h"
#include "HitAction.h"
#include "AttackAction.h"

#include "ComponentBase.h"

class Action;
class MonsterMovement;
class Player;

class ActionMgr
	: public ComponentBase
{
protected:
	// 현재 사용하고 있는 액션
	Action*  m_CurrAction{ nullptr };

	// 이전에 사용했던 액션
	Action*  m_BeforeAction{ nullptr };
	 
public:
	virtual void UpdateState(float fElpasedTime) = 0;

protected:
	// 액션을 변경합니다.
	// 만약 이전 액션과 현재 액션이 동일하다면 변경하지 않습니다.
	bool ChangeAction(Action* );

public:
	ActionMgr(GameObject* pOwner) : ComponentBase(pOwner) { };
	virtual ~ActionMgr() {};
	 
	// Scene의 Update 수행 이전 반드시 호출해야합니다.
	// movement에게서 가속도를 가져와 설정합니다.
	void UpdateVelocity(float fElpasedTime, Movement* movement);
};