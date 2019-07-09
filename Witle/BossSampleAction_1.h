#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// MonsterMovement�� ���ڷ� �޾� ���ӵ��� ó�����ִ� Ŭ����.
// �������� �ʰ� ó���� �Ѵ�.
class BossSampleAction_1
	: public BossMonsterAction
{

public:
	// Update ���� ���� �ݵ�� ȣ��
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	BossSampleAction_1(GameObject* pOwner) : BossMonsterAction(pOwner) { };
	virtual ~BossSampleAction_1() {};

};