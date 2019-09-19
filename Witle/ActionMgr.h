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
	 
	// 이 둘의 ID는 서로 반드시 달라야한다.
	int m_BeforeActionID{ -1 }; // 이전에 사용했던 애니메이션 아이디
	int m_CurrActionID{ 0 }; // 현재 사용하는 애니메이션 아이디

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

	int GetCurrActionID() const { return m_CurrActionID; }
	int GetPrevActionID() const { return m_BeforeActionID; }

	// 이전과 현재의 ID 상태가 달라졌는지 알아낸다. 만약 다르다면 true
	bool isDifferAction() const { return m_BeforeActionID != m_CurrActionID; }

	// m_BeforeActionID 를 m_CurrActionID로 설정하는 함수
	void SetUpPrevActionID()
	{
		m_BeforeActionID = m_CurrActionID;
	}
};