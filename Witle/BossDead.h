#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// �������Ϳ� ��� �׼�
// �������� �ʽ��ϴ�.
// ���� UpdateState �� , �÷��̾�� �Ÿ��� 500 ������ ������ �÷��̾�� ���󰩴ϴ�.
class BossDead
	: public BossMonsterAction
{ 
public:
	virtual void UpdateVelocity(float fElpasedTime, MonsterMovement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	BossDead(GameObject* pOwner ) : BossMonsterAction(pOwner)  { };
	virtual ~BossDead() {};

};