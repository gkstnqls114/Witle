#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// �������Ϳ� ���� �׼�
// �������� �ʽ��ϴ�. 
class BossDead
	: public BossMonsterAction
{ 
public:
	virtual void UpdateVelocity(float fElpasedTime, Movement* movement) override;
	virtual void Init() override {};
	virtual void UpdateState(float fElpasedTime, BossMonsterActionMgr* actionMgr) override;

public:
	BossDead(GameObject* pOwner ) : BossMonsterAction(pOwner, BOSSMONSTER_ACTIONID_DEAD, BOSSMONSTER_ANIMATIONID_DEAD)  { };
	virtual ~BossDead() {};

};