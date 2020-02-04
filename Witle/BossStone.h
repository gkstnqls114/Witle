#pragma once
#include "MonsterAction.h"

class MonsterMovement;

// 보스몬스터용 돌 액션
// 움직이지 않습니다. 
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