#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 보스몬스터용 죽음 액션
// 움직이지 않습니다. 
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