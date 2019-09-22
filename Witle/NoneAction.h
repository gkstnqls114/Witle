#pragma once
#include "PlayerAction.h"
#include "MonsterAction.h"
 
//// �÷��̾� �׼� ���� ���� Ŭ���� ////////////////////////////////
class PlayerNoneAction
	: public PlayerAction
{
public:
	virtual void Init() override {};

	// ���ӵ��� ����.
	// Scene���� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override {};

	virtual void UpdateState(float fElpasedTime, PlayerActionMgr* actionMgr) override {};

public: 
	PlayerNoneAction(Player* pOwner) :
		PlayerAction(pOwner, ENUM_PLAYER_ACTIONID::PLAYER_ACTIONID_ERROR, ENUM_ANIMATIONID::ANIMATIONID_ERROR) {};
	virtual ~PlayerNoneAction() {};
};
//// �÷��̾� �׼� ���� ���� Ŭ���� ////////////////////////////////



//// �⺻ ���� �׼� ���� ���� Ŭ���� ////////////////////////////////
class GeneralMonsterNoneAction
	: public GeneralMonsterAction
{
public:
	virtual void Init() override {};

	// ���ӵ��� ����.
	// Scene���� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override {};

	// �ش� �׼ǻ����� ��, ������ �����ϸ� actionMgr�� ȣ���Ͽ� ���¸� ��ȯ�մϴ�.
	virtual void UpdateState(float fElpasedTime, GeneralMonsterActionMgr* actionMgr) override {};

public:
	GeneralMonsterNoneAction(GameObject* pOwner) :
		GeneralMonsterAction(pOwner, ENUM_GENERALMONSTER_ACTIONID::GENERALMONSTER_ACTIONID_ERROR, ENUM_ANIMATIONID::ANIMATIONID_ERROR) {};
	virtual ~GeneralMonsterNoneAction() {};
};
//// �⺻ ���� �׼� ���� ���� Ŭ���� ////////////////////////////////



//// ���� ���� �׼� ���� ���� Ŭ���� ////////////////////////////////
class BossMonsterNoneAction
	: public BossMonsterAction
{
public:
	virtual void Init() override {};

	// ���ӵ��� ����.
	// Scene���� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override {};

	// �ش� �׼ǻ����� ��, ������ �����ϸ� actionMgr�� ȣ���Ͽ� ���¸� ��ȯ�մϴ�.
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override {};

public:
	BossMonsterNoneAction(GameObject* pOwner) :
		BossMonsterAction(pOwner, ENUM_BOSSMONSTER_ACTIONID::BOSSMONSTER_ACTIONID_ERROR, ENUM_ANIMATIONID::ANIMATIONID_ERROR) {};
	virtual ~BossMonsterNoneAction() {};
};
//// �⺻ ���� �׼� ���� ���� Ŭ���� ////////////////////////////////
