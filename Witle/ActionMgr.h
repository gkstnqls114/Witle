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
	// ���Ŀ� ����� ������ �׼�
	Action*  m_AfterAction{ nullptr };
	 
	// ���� ����� �׼�
	Action*  m_CurrAction{ nullptr };
    
	// ������ ����ߴ� �׼�
	Action*  m_BeforeAction{ nullptr };
	 
	// Before�� Curr�� ID�� ���� �ݵ�� �޶���Ѵ�.
	//int m_BeforeActionID{ -1 }; // ������ ����ߴ� �ִϸ��̼� ���̵� 
	//int m_CurrActionID{ 0 };    // ���Ŀ� ����� �ִϸ��̼� ���̵�
	
public:

	virtual void UpdateState(float fElpasedTime) = 0;

	// ������ ������ Action ID ���°� �޶������� �˾Ƴ���. ���� �ٸ��ٸ� true
	virtual bool isDifferAction() const = 0;

	// ���Ŀ� ����� �׼� ID�� ���� ����ϴ� �׼� ID �ٸ��� �˾Ƴ���. �ٸ��ٸ� true
	virtual bool isDifferAfterAndCurrent() const = 0;

	virtual void Init() = 0;

protected:
	// �׼��� �����մϴ�. 
	bool ChangeAction(Action* );

	// ������ ������ �������� Ȯ���մϴ�.
	// ������ �����ߴ� ActionID�� ���� �ٲ� ActionID�� ���ٸ� false(���氡�������ʴ�)�� ��ȯ�˴ϴ�.
	bool IsPossibleChangeAction(Action* ) const;

public:
	ActionMgr(GameObject* pOwner) : ComponentBase(pOwner) { };
	virtual ~ActionMgr() {};
	 
	// Scene�� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	// movement���Լ� ���ӵ��� ������ �����մϴ�.
	void UpdateVelocity(float fElpasedTime, Movement* movement);

	//int GetCurrActionID() const { return m_CurrActionID; } 
	//int GetPrevActionID() const { return m_BeforeActionID; }
	 
	// m_BeforeAction �� m_AfterAction�� �����ϴ� �Լ�
	void SetUpPrevActionID()
	{
		if (isDifferAfterAndCurrent()) return;

		m_BeforeAction = m_CurrAction;
		m_CurrAction = m_AfterAction;
	}
};