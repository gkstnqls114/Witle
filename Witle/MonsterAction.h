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
	const ENUM_BOSSMONSTER_ACTIONID m_ActionID{ BOSSMONSTER_ACTIONID_ERROR };

public: 
	// �ʱ�ȭ �Լ�.
	virtual void Init() override = 0;

	// ���ӵ��� ����.
	// Scene���� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override = 0;

	// �ش� �׼ǻ����� ��, ������ �����ϸ� actionMgr�� ȣ���Ͽ� ���¸� ��ȯ�մϴ�.
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) = 0;

public:
	BossMonsterAction(GameObject* pOwner, ENUM_BOSSMONSTER_ACTIONID actionID, ENUM_ANIMATIONID animeID)
		: Action(pOwner, animeID) , m_ActionID(actionID) {};
	virtual ~BossMonsterAction() {};
};


// �Ϲ� ���Ϳ� �׼��� �θ� Ŭ����.
// �Ϲ� ���Ϳ� �׼��� �ش� �׼��� ��ӹ޽��ϴ�.
class GeneralMonsterAction
	: public Action
{
public:
	const ENUM_GENERALMONSTER_ACTIONID m_ActionID{ GENERALMONSTER_ACTIONID_ERROR };

public:
	// �ʱ�ȭ �Լ�.
	virtual void Init() override = 0;

	// ���ӵ��� ����.
	// Scene���� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override  = 0;

	// �ش� �׼ǻ����� ��, ������ �����ϸ� actionMgr�� ȣ���Ͽ� ���¸� ��ȯ�մϴ�.
	virtual void UpdateState(float fElpasedTime, GeneralMonsterActionMgr* actionMgr) = 0;

public:
	GeneralMonsterAction(GameObject* pOwner, ENUM_GENERALMONSTER_ACTIONID actionID, ENUM_ANIMATIONID animeID)
		: Action(pOwner, animeID), m_ActionID(actionID){};
	virtual ~GeneralMonsterAction() {};
};