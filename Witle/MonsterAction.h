#pragma once
#include "ComponentBase.h"

class MonsterMovement;
class MonsterActionMgr;
class BossMonsterActionMgr;
class GeneralMonsterActionMgr;
 
// ���� �׼��� �θ� Ŭ����.
class MonsterAction
	: public ComponentBase
{
public:
	// �ʱ�ȭ �Լ�.
	virtual void Init() = 0; 

	// ���ӵ��� ����.
	// Scene���� Update ���� ���� �ݵ�� ȣ���ؾ��մϴ�.
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) = 0; 

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	MonsterAction(GameObject* pOwner) : ComponentBase(pOwner) {};
	virtual ~MonsterAction() {}; 
};


// ���� ���Ϳ� �׼��� �θ� Ŭ����.
// ���� ���Ϳ� �׼��� �ش� �׼��� ��ӹ޽��ϴ�.
class BossMonsterAction
	: public MonsterAction
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
	BossMonsterAction(GameObject* pOwner) : MonsterAction(pOwner) {};
	virtual ~BossMonsterAction() {};
};


// �Ϲ� ���Ϳ� �׼��� �θ� Ŭ����.
// �Ϲ� ���Ϳ� �׼��� �ش� �׼��� ��ӹ޽��ϴ�.
class GeneralMonsterAction
	: public MonsterAction
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
	GeneralMonsterAction(GameObject* pOwner) : MonsterAction(pOwner) {};
	virtual ~GeneralMonsterAction() {};
};