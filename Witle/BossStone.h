#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// �������Ϳ� ��� �׼�
// �������� �ʽ��ϴ�.
// ���� UpdateState �� , �÷��̾�� �Ÿ��� 500 ������ ������ �÷��̾�� ���󰩴ϴ�.
class BossStone
	: public BossMonsterAction
{ 

public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	BossStone(GameObject* pOwner ) : BossMonsterAction(pOwner, BOSSMONSTER_ACTIONID_STONE, BOSSMONSTER_ANIMATIONID_STONE)  { };
	virtual ~BossStone() {};

};