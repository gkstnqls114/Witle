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
	Action*  m_CurrMonsterAction{ nullptr };

	// ������ ����ߴ� �׼�
	Action*  m_BeforeMonsterAction{ nullptr };
	 
public:
	virtual void UpdateState(float fElpasedTime) = 0;

protected:
	// �׼��� �����մϴ�.
	// ���� ���� �׼ǰ� ���� �׼��� �����ϴٸ� �������� �ʽ��ϴ�.
	void ChangeAction(Action* );

public:
	ActionMgr(GameObject* pOwner) : ComponentBase(pOwner) { };
	virtual ~ActionMgr() {};

	// Scene�� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	// movement���Լ� ���ӵ��� ������ �����մϴ�.
	void UpdateVelocity(float fElpasedTime, Movement* movement);
};