#pragma once
#include "Action.h"

class MonsterMovement;
class ActionMgr;
class BossMonsterActionMgr;
class GeneralMonsterActionMgr;
 
// ���� ���Ϳ� �׼��� �θ� Ŭ����.
// ���� ���Ϳ� �׼��� �ش� �׼��� ��ӹ޽��ϴ�.
class BossMonsterAction
	: public Action
{ 
public: 
	// �ʱ�ȭ �Լ�.
	virtual void Init() override = 0;

	// ���ӵ��� ����.
	// Scene���� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override = 0;

	// �ش� �׼ǻ����� ��, ������ �����ϸ� actionMgr�� ȣ���Ͽ� ���¸� ��ȯ�մϴ�.
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) = 0;

public:
	BossMonsterAction(GameObject* pOwner) : Action(pOwner) {};
	virtual ~BossMonsterAction() {};
};


// �Ϲ� ���Ϳ� �׼��� �θ� Ŭ����.
// �Ϲ� ���Ϳ� �׼��� �ش� �׼��� ��ӹ޽��ϴ�.
class GeneralMonsterAction
	: public Action
{
public:
	// �ʱ�ȭ �Լ�.
	virtual void Init() override = 0;

	// ���ӵ��� ����.
	// Scene���� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override  = 0;

	// �ش� �׼ǻ����� ��, ������ �����ϸ� actionMgr�� ȣ���Ͽ� ���¸� ��ȯ�մϴ�.
	virtual void UpdateState(float fElpasedTime, GeneralMonsterActionMgr* actionMgr) = 0;

public:
	GeneralMonsterAction(GameObject* pOwner) : Action(pOwner) {};
	virtual ~GeneralMonsterAction() {};
};