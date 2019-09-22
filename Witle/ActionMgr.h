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

	// ������ ����ߴ� �׼�
	Action*  m_BeforeAction{ nullptr };

	// ���� ����� �׼�
	Action*  m_CurrAction{ nullptr };

	// ���Ŀ� ����� ������ �׼�
	Action*  m_AfterAction{ nullptr };

	  
public:

	virtual void UpdateState(float fElpasedTime) = 0;
	 
	// ���Ŀ� ����� �׼� ID�� ���� ����ϴ� �׼� ID �ٸ��� �˾Ƴ���. �ٸ��ٸ� true
	virtual bool isDifferAfterAndCurrent() const = 0;

	// AfterAction�� �� Ŭ������ �´� NoneAction���� �����Ѵ�.
	virtual void SetUpAfterAction() = 0;

	// AfterAction�� �� Ŭ������ �´� NoneAction�̶�� true�� ��ȯ�Ѵ�.
	virtual bool isAfterNoneAction() = 0;

	virtual void Init() = 0;

protected:
	// �׼��� �����մϴ�. 
	bool ChangeAction(Action * const);

	// ������ ������ �������� Ȯ���մϴ�.
	// ������ �����ߴ� ActionID�� ���� �ٲ� ActionID�� ���ٸ� false(���氡�������ʴ�)�� ��ȯ�˴ϴ�.
	bool isPossibleChangeAction(const Action * const) const;

	bool isSameAfterAction(const Action* const) const;
	bool isSameCurrAction(const Action* const) const;

public:
	ActionMgr(GameObject* pOwner) : ComponentBase(pOwner) { };
	virtual ~ActionMgr() {};
	 
	// Scene�� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	// movement���Լ� ���ӵ��� ������ �����մϴ�.
	virtual void UpdateVelocity(float fElpasedTime, Movement* const movement);
	 
	// m_BeforeAction �� m_CurrAction�� ������ ��
	// m_AfterAction�� m_CurrAction ����.
	void SetUpPrevActionID();
};