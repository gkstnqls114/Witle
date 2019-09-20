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
	// ���� ����ϰ� �ִ� �׼�
	Action*  m_CurrAction{ nullptr };

	// ������ ����ߴ� �׼�
	Action*  m_BeforeAction{ nullptr };
	 
	// �� ���� ID�� ���� �ݵ�� �޶���Ѵ�.
	int m_BeforeActionID{ -1 }; // ������ ����ߴ� �ִϸ��̼� ���̵� 
	int m_CurrActionID{ 0 };    // ���� ����ϴ� �ִϸ��̼� ���̵�

public:
	virtual void UpdateState(float fElpasedTime) = 0;

protected:
	// �׼��� �����մϴ�.
	// ���� ���� �׼ǰ� ���� �׼��� �����ϴٸ� �������� �ʽ��ϴ�.
	bool ChangeAction(Action* );

public:
	ActionMgr(GameObject* pOwner) : ComponentBase(pOwner) { };
	virtual ~ActionMgr() {};
	 
	// Scene�� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	// movement���Լ� ���ӵ��� ������ �����մϴ�.
	void UpdateVelocity(float fElpasedTime, Movement* movement);

	int GetCurrActionID() const { return m_CurrActionID; } 
	int GetPrevActionID() const { return m_BeforeActionID; }

	// ������ ������ ID ���°� �޶������� �˾Ƴ���. ���� �ٸ��ٸ� true
	bool isDifferAction() const { return m_BeforeActionID != m_CurrActionID; }

	// m_BeforeActionID �� m_CurrActionID�� �����ϴ� �Լ�
	void SetUpPrevActionID()
	{
		m_BeforeActionID = m_CurrActionID;
	}
};