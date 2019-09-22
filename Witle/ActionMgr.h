#pragma once 
#include "ComponentBase.h"

class Action;
class MonsterMovement;
class Player;
class Movement;

class ActionMgr
	: public ComponentBase
{ 
protected: 

	// 이전에 사용했던 액션
	Action*  m_BeforeAction{ nullptr };

	// 현재 사용할 액션
	Action*  m_CurrAction{ nullptr };

	// 이후에 사용할 예정인 액션
	Action*  m_AfterAction{ nullptr };

	  
public:

	virtual void UpdateState(float fElpasedTime) = 0;
	 
	// 이후에 사용할 액션 ID와 현재 사용하는 액션 ID 다른지 알아낸다. 다르다면 true
	virtual bool isDifferAfterAndCurrent() const = 0;

	// AfterAction을 각 클래스에 맞는 NoneAction으로 설정한다.
	virtual void SetUpAfterAction() = 0;

	// AfterAction이 각 클래스에 맞는 NoneAction이라면 true를 반환한다.
	virtual bool isAfterNoneAction() = 0;

	virtual void Init() = 0;

protected:
	// 액션을 변경합니다. 
	bool ChangeAction(Action * const);

	// 변경이 가능한 조건인지 확인합니다.
	// 이전에 설정했던 ActionID와 현재 바꿀 ActionID가 같다면 false(변경가능하지않다)가 반환됩니다.
	bool isPossibleChangeAction(const Action * const) const;

	bool isSameAfterAction(const Action* const) const;
	bool isSameCurrAction(const Action* const) const;

public:
	ActionMgr(GameObject* pOwner) : ComponentBase(pOwner) { };
	virtual ~ActionMgr() {};
	 
	// Scene의 Update 수행 이전 반드시 호출해야합니다.
	// movement에게서 가속도를 가져와 설정합니다.
	virtual void UpdateVelocity(float fElpasedTime, Movement* const movement);
	 
	// m_BeforeAction 를 m_CurrAction로 설정한 후
	// m_AfterAction을 m_CurrAction 설정.
	void SetUpPrevActionID();
};