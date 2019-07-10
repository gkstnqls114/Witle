#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// �������Ϳ� ���� �׼� 1�Դϴ�.
// �������� �ʽ��ϴ�.
// ���� UpdateState �� , �÷��̾�� �Ÿ��� 500 ������ ������ �÷��̾�� ���󰩴ϴ�.
class BossSampleAction_1
	: public BossMonsterAction
{

public:
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	BossSampleAction_1(GameObject* pOwner) : BossMonsterAction(pOwner) { };
	virtual ~BossSampleAction_1() {};

};