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
	// ���� ����ϰ� �ִ� �׼�
	Action*  m_CurrAction{ nullptr };
	 
	// ������ ����ߴ� �׼�
	Action*  m_BeforeAction{ nullptr };
	 
	// Before�� Curr�� ID�� ���� �ݵ�� �޶���Ѵ�.
	int m_BeforeActionID{ -1 }; // ������ ����ߴ� �ִϸ��̼� ���̵� 
	int m_CurrActionID{ 0 };    // ���Ŀ� ����� �ִϸ��̼� ���̵�
	
public:

	virtual void UpdateState(float fElpasedTime) = 0;

	// ������ ������ Action ID ���°� �޶������� �˾Ƴ���. ���� �ٸ��ٸ� true
	virtual bool isDifferAction() const = 0;

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

	int GetCurrActionID() const { return m_CurrActionID; } 
	int GetPrevActionID() const { return m_BeforeActionID; }

	// m_BeforeActionID �� m_CurrActionID�� �����ϴ� �Լ�
	void SetUpPrevActionID()
	{
		m_BeforeActionID = m_CurrActionID;
	}
};