#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// �������Ϳ� ���� �׼� 2�Դϴ�.
// �÷��̾ ���󰩴ϴ�.
// ���� UpdateState �� , �÷��̾�� �Ÿ��� 500 ������ �Ѿ�� �÷��̾�� ���󰩴ϴ�.
class BossSampleAction_2
	: public BossMonsterAction
{

public:
	// Update ���� ���� �ݵ�� ȣ��
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	BossSampleAction_2(GameObject* pOwner) : BossMonsterAction(pOwner) { };
	virtual ~BossSampleAction_2() {};

};