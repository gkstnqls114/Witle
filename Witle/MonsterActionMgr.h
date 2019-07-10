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
	// ���� ����ϰ� �ִ� �׼�
	MonsterAction*  m_CurrMonsterAction{ nullptr };

	// ������ ����ߴ� �׼�
	MonsterAction*  m_BeforeMonsterAction{ nullptr };
	 
public:
	virtual void UpdateState(float fElpasedTime) = 0;

protected:
	// �׼��� �����մϴ�.
	// ���� ���� �׼ǰ� ���� �׼��� �����ϴٸ� �������� �ʽ��ϴ�.
	void ChangeAction(MonsterAction* );

public:
	MonsterActionMgr(GameObject* pOwner) : ComponentBase(pOwner) { };
	virtual ~MonsterActionMgr() {};

	// Scene�� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	// movement���Լ� ���ӵ��� ������ �����մϴ�.
	void UpdateVelocity(float fElpasedTime, MonsterMovement* movement);
};