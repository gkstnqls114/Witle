#pragma once
#include "ComponentBase.h"

class MonsterMovement;
class MonsterActionMgr;

// MonsterMovement�� ���ڷ� �޾� ���ӵ��� ó�����ִ� Ŭ����.
class MonsterAction
	: public ComponentBase
{
public:
	// Update ���� ���� �ݵ�� ȣ��
	virtual void Init() = 0;
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) = 0;
	virtual void UpdateState(float fElpasedTime, MonsterActionMgr* actionMgr) = 0;

public:
	virtual void ReleaseObjects() override {};
	virtual void ReleaseUploadBuffers() override {};

public:
	MonsterAction(GameObject* pOwner) : ComponentBase(pOwner) {};
	virtual ~MonsterAction() {};
	 
	XMFLOAT3 AlreadyUpdate(float) {};
	 
	void MoveVelocity(const XMFLOAT3& shift) {};

	void ReduceVelocity(float) {}; 
};


class BossMonsterAction
	: public MonsterAction
{ 
public:
	// Update ���� ���� �ݵ�� ȣ��
	virtual void Init() = 0;
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) = 0;
	virtual void UpdateState(float fElpasedTime, MonsterActionMgr* actionMgr) = 0;

public:
	BossMonsterAction(GameObject* pOwner) : MonsterAction(pOwner) {};
	virtual ~BossMonsterAction() {};
};