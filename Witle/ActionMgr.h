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
	// 현재 사용하고 있는 액션
	Action*  m_CurrAction{ nullptr };
	 
	// 이전에 사용했던 액션
	Action*  m_BeforeAction{ nullptr };
	 
	// Before과 Curr의 ID는 서로 반드시 달라야한다.
	int m_BeforeActionID{ -1 }; // 이전에 사용했던 애니메이션 아이디 
	int m_CurrActionID{ 0 };    // 이후에 사용할 애니메이션 아이디
	
public:

	virtual void UpdateState(float fElpasedTime) = 0;

	// 이전과 현재의 Action ID 상태가 달라졌는지 알아낸다. 만약 다르다면 true
	virtual bool isDifferAction() const = 0;

	virtual void Init() = 0;

protected:
	// 액션을 변경합니다. 
	bool ChangeAction(Action* );

	// 변경이 가능한 조건인지 확인합니다.
	// 이전에 설정했던 ActionID와 현재 바꿀 ActionID가 같다면 false(변경가능하지않다)가 반환됩니다.
	bool IsPossibleChangeAction(Action* ) const;

public:
	ActionMgr(GameObject* pOwner) : ComponentBase(pOwner) { };
	virtual ~ActionMgr() {};
	 
	// Scene의 Update 수행 이전 반드시 호출해야합니다.
	// movement에게서 가속도를 가져와 설정합니다.
	void UpdateVelocity(float fElpasedTime, Movement* movement);

	int GetCurrActionID() const { return m_CurrActionID; } 
	int GetPrevActionID() const { return m_BeforeActionID; }

	// m_BeforeActionID 를 m_CurrActionID로 설정하는 함수
	void SetUpPrevActionID()
	{
		m_BeforeActionID = m_CurrActionID;
	}
};